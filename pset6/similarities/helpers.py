from enum import Enum


class Operation(Enum):
    """Operations"""

    DELETED = 1
    INSERTED = 2
    SUBSTITUTED = 3

    def __str__(self):
        return str(self.name.lower())


def distances(a, b):
    """Calculate edit distance from a to b"""

    # SETUP 2D LIST
    matrix = [[(None, None) for y in range(len(b)+1)] for x in range(len(a)+1)]
    # ADD BASE CASES
    matrix[0][0] = (0, None)
    for x in range(len(matrix[1:])):
        matrix[x+1][0] = (x+1, Operation.DELETED)
    for y in range(len(matrix[0][1:])):
        matrix[0][y+1] = (y+1, Operation.INSERTED)

    # FILL IN OTHER ENTRIES
    # Loop function here uses fewer stack calls and remains O(N) so would be preffered choice?
    # Spec says recursive might be useful, mind...
    for i in range(1, len(a) + 1):
        for j in range(1, len(b) + 1):
            delete = matrix[i-1][j][0] + 1
            insert = matrix[i][j-1][0] + 1
            sub = matrix[i-1][j-1][0] if a[i-1] == b[j-1] else matrix[i-1][j-1][0] + 1  # nopep8

            if(delete <= insert and delete <= sub):
                update = (delete, Operation.DELETED)
            elif(insert <= delete and insert <= sub):
                update = (insert, Operation.INSERTED)
            else:
                update = (sub, Operation.SUBSTITUTED)

            matrix[i][j] = update

    return matrix
