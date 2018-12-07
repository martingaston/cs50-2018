import os

from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

import queries

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


# Custom filter
@app.template_filter('usd')
def format_usd(s):
    return usd(s)


# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    portfolio = queries.select_portfolio(session["user_id"])
    share_total = 0
    for i in range(len(portfolio)):
        portfolio[i]["live_price"] = lookup(portfolio[i]["symbol"])["price"]
        share_total += portfolio[i]["live_price"] * portfolio[i]["shares"]
    current_cash = queries.select_current_cash(session["user_id"])

    return render_template("index.html", portfolio=portfolio, current_cash=current_cash, total=share_total + current_cash)


@app.route("/password", methods=["GET", "POST"])
@login_required
def password():
    """Let user change password"""

    if request.method == "POST":

        if not request.form.get("password"):
            return apology("Must enter current password", 400)

        if not request.form.get("new_password"):
            return apology("Must enter new password", 400)

        if not request.form.get("confirm_new_password"):
            return apology("Must enter password confirmation", 400)

        if request.form.get("confirm_new_password") != request.form.get("new_password"):
            return apology("Passwords must match", 400)

        # if form validation passes, select user from database
        user = queries.select_userid(session["user_id"])

        if not user:
            return apology("Username does not exist", 400)

        if not check_password_hash(user[0]["hash"], request.form.get("password")):
            return apology("Incorrect password", 400)

        # update password & logout

        queries.update_password(
            user[0]["id"], generate_password_hash(request.form["new_password"]))

        flash("Updated password. Please login again.")
        return redirect(url_for("logout"))

    else:
        return render_template("password.html")


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("No symbol specified", 400)
        elif not request.form.get("shares"):
            return apology("No shares specified", 400)

        try:
            shares_to_sell = int(request.form.get("shares"))
        except ValueError:
            return apology("Invalid share number given", 400)

        if shares_to_sell < 1:
            return apology("Need to sell at least +1 share(s)", 400)

        # get current stock price and user's current cash level
        quote = lookup(request.form.get("symbol"))
        if not quote:
            return apology("Invalid symbol specified", 400)

        total_cash = queries.select_current_cash(session["user_id"])
        new_total = total_cash - \
            shares_to_sell * float(quote["price"])

        # check the user can afford to buy
        if new_total < 0:
            return apology("Not enough money!", 400)

        # add symbol to database if it doesn't currently exist
        if not queries.select_symbol(quote["symbol"]):
            queries.insert_company(symbol=quote["symbol"], name=quote["name"])

        # complete the transaction and redirect to /
        queries.insert_transaction(
            user=session["user_id"], symbol=quote["symbol"], cost=quote["price"], shares=request.form["shares"], new_total=new_total)

        flash("Bought!")
        return redirect("/")

    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = queries.select_history(session["user_id"])
    return render_template("history.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = queries.select_username(request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    flash("Logged Out!")
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("Must specify stock symbol", 400)
        stock = lookup(request.form.get("symbol"))
        if not stock:
            return apology("Incorrect stock symbol", 400)

        return render_template("quoted.html", stock=stock)

    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    if request.method == "POST":
        if not request.form.get("username"):
            return apology("Must supply a username", 400)

        if queries.select_username(request.form["username"]):
            return apology("Username already registered", 400)

        if not request.form.get("password"):
            return apology("Must supply a password", 400)

        if request.form.get("password") != request.form.get("confirmation"):
            return apology("Passwords must match!", 400)

        queries.insert_user(username=request.form["username"], hash=generate_password_hash(
            request.form["password"]))

        flash("Registration Completed. Please Log In.")
        return redirect(url_for("login"))

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("No stock name specified", 400)

        if not request.form.get("shares"):
            return apology("No shares specified", 400)

        # cast user's desired amount of shares to sell to int
        share_total = int(request.form["shares"])

        # get user's current amount of shares in stock
        current_shares = queries.select_current_shares(
            id=session["user_id"], symbol=request.form["symbol"])

        # error if the user is trying to sell more shares than they own
        if int(share_total) > current_shares[0]["shares"] or not current_shares:
            return apology("You don't have that many shares!", 400)

        # get live price of stock
        quote = lookup(request.form["symbol"])["price"]

        # update user's cash amount + price of sold stock
        current_cash = queries.select_current_cash(session["user_id"])
        updated_cash = current_cash + int(share_total) * quote

        # run the transaction and redirect to index
        queries.insert_transaction(
            user=session["user_id"], symbol=request.form["symbol"], cost=quote, shares=-share_total, new_total=updated_cash)

        flash("Sold!")
        return redirect(url_for("index"))

    else:
        symbols = queries.select_portfolio_symbols(session["user_id"])
        return render_template("sell.html", symbols=symbols)


def errorhandler(e):
    """Handle error"""
    return apology(e.name, e.code)


# listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
