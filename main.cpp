#define _CRT_SECURE_NO_WARNINGS
#include <iostream>

struct Node {
    int i, j;
    int previousValue;
    int memoryOffset, childrenCount;
    int childrenOffset;
    int currentPlayer;
    bool expanded, and_type;
    int state;
    long long proof, disproof;
    int parent, nodeIndex;

    void init(int i, int j, int previousValue, int memoryOffset, int currentPlayer, bool and_type, int parent) {
        this->i = i;
        this->j = j;
        this->previousValue = previousValue;
        this->currentPlayer = currentPlayer;
        this->memoryOffset = memoryOffset;
        this->and_type = and_type;
        this->parent = parent;
        expanded = false;
        childrenCount = 0;
        state = 3;
        childrenOffset = 0;
    }
};

struct NMKSolver {
    int n, m, k;
    int** data;
    const static int maxNodes = INT_MAX / sizeof(Node) / 10;
    static Node* gameTree;

    NMKSolver(int n, int m, int k) {
        this->n = n;
        this->m = m;
        this->k = k;
        data = new int* [n];
        for (int i = 0; i < n; i++)
            data[i] = new int[m];
    }

    void load() {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                scanf("%d", &data[i][j]);
    }

    void printData() const {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                printf("%d ", data[i][j]);
            }
            printf("\n");
        }
    }

    bool isRowWinning(int y, int x) const {
        if (x + k > m)
            return false;
        for (int i = x + 1; i < x + k; i++)
            if (data[y][i] != data[y][i - 1])
                return false;
        return true;
    }

    bool isColumnWinning(int y, int x) const {
        if (y + k > n)
            return false;
        for (int i = y + 1; i < y + k; i++)
            if (data[i][x] != data[i - 1][x])
                return false;
        return true;
    }

    bool isLeftDiagonalWinning(int y, int x) const {
        if (y + k > n || x + k > m)
            return false;
        for (int i = 1; i < k; i++)
            if (data[y + i][x + i] != data[y + i - 1][x + i - 1])
                return false;
        return true;
    }

    bool isRightDiagonalWinning(int y, int x) const {
        if (y + k > n || x - k + 1 < 0)
            return false;
        for (int i = 1; i < k; i++)
            if (data[y + i][x - i] != data[y + i - 1][x - i + 1])
                return false;
        return true;
    }

    int isGameWon() const {
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                if (data[i][j] && (j == 0 || data[i][j - 1] != data[i][j]))
                    if (isRowWinning(i, j))
                        return data[i][j];
                if (data[i][j] && (i == 0 || data[i - 1][j] != data[i][j]))
                    if (isColumnWinning(i, j))
                        return data[i][j];
                if (data[i][j] && (i == 0 || j == 0 || data[i - 1][j - 1] != data[i][j]))
                    if (isLeftDiagonalWinning(i, j))
                        return data[i][j];
                if (data[i][j] && (i == 0 || j == m - 1 || data[i - 1][j + 1] != data[i][j]))
                    if (isRightDiagonalWinning(i, j))
                        return data[i][j];
            }
        return 0;
    }

    int countRight(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && x + i < m; i++) {
            if (data[y][x + i] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int countLeft(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && x - i >= 0; i++) {
            if (data[y][x - i] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int countTop(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && y - i >= 0; i++) {
            if (data[y - i][x] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int countBottom(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && y + i < n; i++) {
            if (data[y + i][x] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int countRightBottomDiagonal(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && y + i < n && x + i < m; i++) {
            if (data[y + i][x + i] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int countLeftBottomDiagonal(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && y + i < n && x - i >= 0; i++) {
            if (data[y + i][x - i] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int countLeftTopDiagonal(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && y - i >= 0 && x - i >= 0; i++) {
            if (data[y - i][x - i] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int countRightTopDiagonal(int y, int x, int move) const {
        int current = 0;
        for (int i = 0; i < k && y - i >= 0 && x + i < m; i++) {
            if (data[y - i][x + i] == move)
                current++;
            else
                return current;
        }
        return current;
    }

    int isMoveWinning(int y, int x, int move) {
        int tmp = data[y][x];
        data[y][x] = move;
        if ((countRight(y, x, move) + countLeft(y, x, move) > k) ||
            (countTop(y, x, move) + countBottom(y, x, move) > k) ||
            (countLeftTopDiagonal(y, x, move) + countRightBottomDiagonal(y, x, move) > k) ||
            (countRightTopDiagonal(y, x, move) + countLeftBottomDiagonal(y, x, move) > k))
        {
            data[y][x] = tmp;
            return move;
        }
        data[y][x] = tmp;
        return 0;
    }

    void generateAll(int player, int mode = 0) {
        if (isGameWon()) {
            printf("0\n");
            return;
        }
        int possibleMoves = 0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                if (data[i][j] == 0) {
                    possibleMoves++;
                    if (mode == 1 && isMoveWinning(i, j, player)) {
                        printf("1\n");
                        data[i][j] = player;
                        printData();
                        return;
                    }
                }
            }
        printf("%d\n", possibleMoves);

        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                if (data[i][j] == 0) {
                    int tmp = data[i][j];
                    data[i][j] = player;
                    printData();
                    data[i][j] = tmp;
                }
            }
    }

    void applyChanges(Node* node) {
        if (node->parent == -1)
            return;
        int i = node->i;
        int j = node->j;
        data[i][j] = node->currentPlayer;
    }

    void revertChanges(Node* node) {
        if (node->parent == -1)
            return;
        int i = node->i;
        int j = node->j;
        data[i][j] = node->previousValue;
    }

    void evaluate(Node* node, int player) {
        if (node->parent == -1) {
            int result = isGameWon();
            if (result == player)
                result = 1;
            else if (result != 0)
                result = 0;
            else
                result = 2;
            node->state = result;
        }
        else {
            int i = node->i;
            int j = node->j;
            int result = isMoveWinning(i, j, node->currentPlayer);
            if (result == player)
                result = 1;
            else if (result != 0)
                result = 0;
            else
                result = 2;
            node->state = result;
        }
    }

    void setProofAndDisproofNumbers(Node* node) {
        if (node->expanded && node->childrenCount > 0) {
            if (node->and_type) {
                node->proof = 0;
                node->disproof = INT_MAX;
                for (int i = 0; i < node->childrenCount; i++) {
                    Node& child = gameTree[node->childrenOffset + i];
                    node->proof += child.proof;
                    if (child.disproof < node->disproof) {
                        node->disproof = child.disproof;
                        if (node->disproof == 0)
                            break;
                    }
                }
            }
            else {
                node->proof = INT_MAX;
                node->disproof = 0;
                for (int i = 0; i < node->childrenCount; i++) {
                    Node& child = gameTree[node->childrenOffset + i];
                    node->disproof += child.disproof;
                    if (child.proof < node->proof) {
                        node->proof = child.proof;
                        if (node->proof == 0)
                            break;
                    }
                }
            }
        }
        else {
            if (node->state == 0) { // False
                node->proof = INT_MAX;
                node->disproof = 0;
            }
            else if (node->state == 1) { // True
                node->proof = 0;
                node->disproof = INT_MAX;
            }
            else { // Unknown
                node->proof = 1;
                node->disproof = 1;
            }
        }
    }

    Node* selectMostProvingNode(Node* node) {
        while (node->expanded && node->childrenCount > 0) {
            if (node->and_type) {
                for (int i = 0; i < node->childrenCount; i++) {
                    Node& child = gameTree[node->childrenOffset + i];
                    if (node->disproof == child.disproof) {
                        applyChanges(&child);
                        node = &child;
                        break;
                    }
                }
            }
            else {
                for (int i = 0; i < node->childrenCount; i++) {
                    Node* child = &gameTree[node->childrenOffset + i];
                    if (node->proof == child->proof) {
                        applyChanges(child);
                        node = child;
                        break;
                    }
                }
            }
        }
        return node;
    }


    void expandNode(Node* node, int& currentOffset, int player) {
        int nextPlayer = (node->currentPlayer + 1) % 2;
        if (nextPlayer == 0)
            nextPlayer = 2;
        node->childrenOffset = currentOffset;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) {
                if (data[i][j] == 0) {
                    node->childrenCount++;
                    gameTree[currentOffset].init(i, j, data[i][j], currentOffset, nextPlayer, !node->and_type, node->memoryOffset);
                    currentOffset++;
                }
            }
        if (node->childrenCount == 0) {
            node->state = 0;
            node->disproof = 0; // draw
        }
        for (int i = 0; i < node->childrenCount; i++) {
            Node* child = &gameTree[node->childrenOffset + i];
            evaluate(child, player);
            setProofAndDisproofNumbers(child);

            if ((node->and_type && child->disproof == 0) || (!node->and_type && child->proof == 0)) {
                break;
            }
        }
        node->expanded = true;
    }

    Node* updateAncestors(Node* mostProvingNode) {
        while (1) {
            long long proof = mostProvingNode->proof;
            long long disproof = mostProvingNode->disproof;
            setProofAndDisproofNumbers(mostProvingNode);

            if (proof == mostProvingNode->proof && disproof == mostProvingNode->disproof)
                return mostProvingNode;

            if (mostProvingNode->parent == -1)
                return mostProvingNode;

            revertChanges(mostProvingNode);
            mostProvingNode = &gameTree[mostProvingNode->parent];
        }
    }

    bool PN(int player, int currentPlayer) {
        bool and_type = (player == currentPlayer);
        gameTree[0].init(0, 0, data[0][0], 0, currentPlayer, and_type, -1); //root makes no changes to game
        Node* root = &gameTree[0];
        int currentOffset = 1;

        evaluate(root, player);
        setProofAndDisproofNumbers(root);
        Node* currentNode = root;

        while (root->proof != 0 && root->disproof != 0) {
            Node* mostProvingNode = selectMostProvingNode(currentNode);
            expandNode(mostProvingNode, currentOffset, player);
            currentNode = updateAncestors(mostProvingNode);
        }

        long long proof = root->proof;

        while (currentNode->parent != -1) {
            revertChanges(currentNode);
            currentNode = &gameTree[currentNode->parent];
        }
        if (proof == 0)
            return true;
        else
            return false;
    }

    ~NMKSolver() {
        for (int i = 0; i < n; i++)
            delete[] data[i];
        delete[] data;
    }

};

Node* NMKSolver::gameTree = nullptr;

int main() {
    int n, m, k, player;
    char command[50];
    NMKSolver::gameTree = new Node[NMKSolver::maxNodes];
    while (scanf("%s %d %d %d %d", command, &n, &m, &k, &player) > 0) {
        NMKSolver solver(n, m, k);
        solver.load();
        if (strcmp("GEN_ALL_POS_MOV", command) == 0)
            solver.generateAll(player);
        else if (strcmp("GEN_ALL_POS_MOV_CUT_IF_GAME_OVER", command) == 0)
            solver.generateAll(player, 1);
        else {
            player = (player + 1) % 2;
            if (player == 0)
                player = 2;
            if (solver.PN(1, player))
                printf("FIRST_PLAYER_WINS\n");
            else if (solver.PN(2, player))
                printf("SECOND_PLAYER_WINS\n");
            else
                printf("BOTH_PLAYERS_TIE\n");
        }
    }
    delete[] NMKSolver::gameTree;
}