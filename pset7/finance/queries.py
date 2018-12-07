from cs50 import SQL

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


def select_portfolio(id):
    """Select current portfolio of logged in user"""
    query = db.execute(
        "SELECT companies.symbol, companies.name, SUM(transactions.shares) AS shares FROM transactions INNER JOIN companies ON companies.id = transactions.company WHERE user = :id GROUP BY companies.symbol HAVING SUM(transactions.shares) > 0", id=id)
    return wrap_query(query)


def select_current_shares(id, symbol):
    """Return the user's current amount of shares for a supplied symbol"""
    query = db.execute(
        "SELECT companies.symbol, SUM(transactions.shares) as shares FROM transactions INNER JOIN companies ON companies.id = transactions.company WHERE transactions.user = :id AND companies.symbol = :symbol GROUP BY companies.symbol", id=id, symbol=symbol)
    return wrap_query(query)


def select_current_cash(id):
    """Return logged in user's current cash amount"""
    query = db.execute("SELECT cash FROM users WHERE id = :id", id=id)[
        0]["cash"]
    return wrap_query(query)


def select_symbol(symbol):
    """Check to see if symbol already exists in database"""
    query = db.execute(
        "SELECT symbol FROM companies WHERE symbol = :symbol", symbol=symbol)
    return wrap_query(query)


def select_history(id):
    query = db.execute("SELECT companies.symbol, companies.name, transactions.shares, transactions.cost, transactions.datetime FROM transactions INNER JOIN companies ON transactions.company = companies.id WHERE user = :id",
                       id=id)
    return wrap_query(query)


def select_username(username):
    """Select username from database"""
    query = db.execute("SELECT * FROM users WHERE username = :username",
                       username=username)
    return wrap_query(query)


def select_userid(id):
    """Select username from database"""
    query = db.execute("SELECT * FROM users WHERE id = :id", id=id)
    return wrap_query(query)


def select_portfolio_symbols(id):
    """Select all symbols from database where current user currently owns more than 0 shares"""
    query = db.execute(
        "SELECT symbol FROM transactions INNER JOIN companies ON transactions.company = companies.id WHERE user = :id GROUP BY name HAVING SUM(transactions.shares) > 0", id=id)
    return wrap_query(query)


def insert_company(symbol, name):
    """Insert a new company into the database"""
    query = db.execute(
        "INSERT INTO companies (symbol, name) VALUES (:symbol, :name)", symbol=symbol, name=name)
    return wrap_query(query)


def insert_user(username, hash):
    """Insert a new user into the database"""
    query = db.execute(
        "INSERT INTO users (username, hash) VALUES (:username, :hash)", username=username, hash=hash)
    return wrap_query(query)


def insert_transaction(user, symbol, cost, shares, new_total):
    """Insert new transaction into database and update user's total cash"""
    # CS50 SQL library doesn't seem to support BEGIN TRANSACTION and COMMIT :(
    db.execute("INSERT INTO transactions (user, company, cost, shares, datetime) VALUES (:user, (SELECT id FROM companies WHERE symbol = :symbol), :cost, :shares, DATETIME('now', 'localtime'))",
               user=user, symbol=symbol, cost=cost, shares=shares)
    query = db.execute("UPDATE users SET cash = :total WHERE id = :user",
                       total=new_total, user=user)
    return wrap_query(query)


def update_password(user, newhash):
    """Update user's password to new hash"""
    query = db.execute(
        "UPDATE users SET hash = :hash WHERE id = :user", hash=newhash, user=user)
    return wrap_query(query)


def wrap_query(query):
    if query:
        return query
    else:
        return ""
