#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <map>
#include <cstring>

using namespace std;
using namespace sf;

struct Position
{
    int row;
    int column;
};

class ChessBoard;

enum GameStatus
{
    ONGOING,
    CHECKMATE,
    STALEMATE
};

class Piece
{
protected:
    string colour;
    char symbol;

public:
    Piece(const string& c) : colour(c) {}
    virtual ~Piece() {}
    string getColour() const { return colour; }
    char getSymbol() const { return symbol; }
    virtual bool isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const = 0;
    virtual string getType() const = 0;
};

class Pawn : public Piece
{
public:
    Pawn(const string& c) : Piece(c) { symbol = (c == "white") ? 'P' : 'p'; }
    bool isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const override;
    string getType() const override { return "pawn"; }
};

class Rook : public Piece
{
public:
    Rook(const string& c) : Piece(c) { symbol = (c == "white") ? 'R' : 'r'; }
    bool isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const override;
    string getType() const override { return "rook"; }
};

class Knight : public Piece
{
public:
    Knight(const string& c) : Piece(c) { symbol = (c == "white") ? 'N' : 'n'; }
    bool isMoveValid(const Position& from, const Position& to, const ChessBoard*) const override
    {
        int dx = abs(from.row - to.row);
        int dy = abs(from.column - to.column);
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }
    string getType() const override { return "knight"; }
};

class Bishop : public Piece
{
public:
    Bishop(const string& c) : Piece(c) { symbol = (c == "white") ? 'B' : 'b'; }
    bool isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const override;
    string getType() const override { return "bishop"; }
};

class Queen : public Piece
{
public:
    Queen(const string& c) : Piece(c) { symbol = (c == "white") ? 'Q' : 'q'; }
    bool isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const override;
    string getType() const override { return "queen"; }
};

class King : public Piece
{
public:
    King(const string& c) : Piece(c) { symbol = (c == "white") ? 'K' : 'k'; }
    bool isMoveValid(const Position& from, const Position& to, const ChessBoard*) const override
    {
        int dx = abs(from.row - to.row);
        int dy = abs(from.column - to.column);
        return (dx <= 1 && dy <= 1) && (dx + dy > 0);
    }
    string getType() const override { return "king"; }
};

class ChessBoard
{
public:
    Piece* board[8][8];
    string turn;

    ChessBoard() : turn("white")
    {
        memset(board, 0, sizeof(board));
        setupBoard();
    }
    ~ChessBoard()
    {
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                delete board[i][j];
    }

    void setupBoard();
    Piece* getPiece(const Position& pos) const;
    bool isPathClear(const Position& from, const Position& to) const;
    bool isKingInCheck(const string& color) const;
    bool move(const Position& from, const Position& to);
    bool hasAnyLegalMoves(const string& color);
    GameStatus getGameStatus();
};

bool Pawn::isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const
{
    if (from.row == to.row && from.column == to.column)
        return false;
    int direction = (colour == "white") ? -1 : 1;
    int startRow = (colour == "white") ? 6 : 1;
    if (from.column == to.column)
    {
        if (to.row == from.row + direction && !board->getPiece(to))
            return true;
        if (from.row == startRow && to.row == from.row + 2 * direction &&
            !board->getPiece(to) && !board->getPiece({ from.row + direction, from.column }))
            return true;
    }
    if (abs(to.column - from.column) == 1 && to.row == from.row + direction)
    {
        Piece* target = board->getPiece(to);
        if (target && target->getColour() != colour)
            return true;
    }
    return false;
}

bool Rook::isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const
{
    if (from.row == to.row || from.column == to.column)
        return board->isPathClear(from, to);
    return false;
}

bool Bishop::isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const
{
    if (abs(from.row - to.row) == abs(from.column - to.column))
        return board->isPathClear(from, to);
    return false;
}

bool Queen::isMoveValid(const Position& from, const Position& to, const ChessBoard* board) const
{
    int dr = abs(from.row - to.row);
    int dc = abs(from.column - to.column);
    if (from.row == to.row || from.column == to.column || dr == dc)
        return board->isPathClear(from, to);
    return false;
}

void ChessBoard::setupBoard()
{
    for (int col = 0; col < 8; ++col)
    {
        board[1][col] = new Pawn("black");
        board[6][col] = new Pawn("white");
    }
    board[0][0] = new Rook("black");
    board[0][7] = new Rook("black");
    board[7][0] = new Rook("white");
    board[7][7] = new Rook("white");
    board[0][1] = new Knight("black");
    board[0][6] = new Knight("black");
    board[7][1] = new Knight("white");
    board[7][6] = new Knight("white");
    board[0][2] = new Bishop("black");
    board[0][5] = new Bishop("black");
    board[7][2] = new Bishop("white");
    board[7][5] = new Bishop("white");
    board[0][3] = new Queen("black");
    board[7][3] = new Queen("white");
    board[0][4] = new King("black");
    board[7][4] = new King("white");
}

Piece* ChessBoard::getPiece(const Position& pos) const
{
    if (pos.row < 0 || pos.row > 7 || pos.column < 0 || pos.column > 7)
        return nullptr;
    return board[pos.row][pos.column];
}

bool ChessBoard::isPathClear(const Position& from, const Position& to) const
{
    int dr = (to.row > from.row) ? 1 : (to.row < from.row) ? -1
        : 0;
    int dc = (to.column > from.column) ? 1 : (to.column < from.column) ? -1
        : 0;
    int r = from.row + dr, c = from.column + dc;
    while (r != to.row || c != to.column)
    {
        if (board[r][c])
            return false;
        r += dr;
        c += dc;
    }
    return true;
}

bool ChessBoard::isKingInCheck(const string& color) const
{
    Position kingPos{ -1, -1 };
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
        {
            Piece* p = board[r][c];
            if (p && p->getType() == "king" && p->getColour() == color)
            {
                kingPos = { r, c };
                break;
            }
        }
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
        {
            Piece* p = board[r][c];
            if (p && p->getColour() != color)
            {
                if (p->isMoveValid({ r, c }, kingPos, this))
                {
                    string t = p->getType();
                    if ((t == "rook" || t == "bishop" || t == "queen") && !isPathClear({ r, c }, kingPos))
                        continue;
                    return true;
                }
            }
        }
    return false;
}

bool ChessBoard::move(const Position& from, const Position& to)
{
    Piece* p = getPiece(from);
    if (!p || p->getColour() != turn)
        return false;
    if (!p->isMoveValid(from, to, this))
        return false;
    Piece* dest = getPiece(to);
    if (dest && dest->getColour() == p->getColour())
        return false;
    board[to.row][to.column] = p;
    board[from.row][from.column] = nullptr;
    if (isKingInCheck(turn))
    {
        board[from.row][from.column] = p;
        board[to.row][to.column] = dest;
        return false;
    }
    delete dest;
    if (p->getType() == "pawn" && (to.row == 0 || to.row == 7))
    {
        delete board[to.row][to.column];
        board[to.row][to.column] = new Queen(p->getColour());
    }
    turn = (turn == "white") ? "black" : "white";
    return true;
}

bool ChessBoard::hasAnyLegalMoves(const string& color)
{
    for (int r = 0; r < 8; ++r)
        for (int c = 0; c < 8; ++c)
        {
            Piece* p = board[r][c];
            if (p && p->getColour() == color)
            {
                for (int tr = 0; tr < 8; ++tr)
                    for (int tc = 0; tc < 8; ++tc)
                    {
                        Position from{ r, c }, to{ tr, tc };
                        if (p->isMoveValid(from, to, this))
                        {
                            Piece* dest = board[tr][tc];
                            board[tr][tc] = p;
                            board[r][c] = nullptr;
                            bool inCheck = isKingInCheck(color);
                            board[r][c] = p;
                            board[tr][tc] = dest;
                            if (!inCheck)
                                return true;
                        }
                    }
            }
        }
    return false;
}

GameStatus ChessBoard::getGameStatus()
{
    if (hasAnyLegalMoves(turn))
        return ONGOING;
    if (isKingInCheck(turn))
        return CHECKMATE;
    return STALEMATE;
}

int main()
{
    RenderWindow window(VideoMode(640, 640), "Chess - SFML");
    ChessBoard board;
    map<string, Texture> textures;
    const string names[] = { "king", "queen", "rook", "bishop", "knight", "pawn" };
    const string cols[2] = { "w", "b" };
    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 6; ++j)
        {
            string key = cols[i] + "_" + names[j];
            Texture tex;
            if (!tex.loadFromFile("assets/" + key + ".png"))
                cerr << "Missing asset: " << key << ".png\n";
            textures[key] = move(tex);
        }
    Sprite grabbed;
    bool dragging = false;
    Position from{}, to{};
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                from.row = event.mouseButton.y / 80;
                from.column = event.mouseButton.x / 80;
                Piece* p = board.getPiece(from);
                if (p && p->getColour() == board.turn)
                {
                    string key = string(1, p->getColour()[0]) + "_" + p->getType();
                    grabbed.setTexture(textures[key]);
                    grabbed.setPosition(from.column * 80, from.row * 80);
                    dragging = true;
                }
            }
            if (event.type == Event::MouseButtonReleased && event.mouseButton.button == Mouse::Left && dragging)
            {
                to.row = event.mouseButton.y / 80;
                to.column = event.mouseButton.x / 80;
                dragging = false;
                if (board.move(from, to))
                {
                    GameStatus status = board.getGameStatus();
                    if (status != ONGOING)
                    {
                        string msg;
                        if (status == CHECKMATE)
                        {
                            string winner = (board.turn == "white") ? "black" : "white";
                            msg = "Checkmate! " + winner + " wins!";
                        }
                        else
                        {
                            msg = "Stalemate! Draw game.";
                        }
                        cout << msg << "\n";
                        window.close();
                    }
                }
            }
        }
        window.clear(Color::White);
        for (int r = 0; r < 8; ++r)
            for (int c = 0; c < 8; ++c)
            {
                RectangleShape square(Vector2f(80.f, 80.f));
                square.setPosition(c * 80.f, r * 80.f);
                square.setFillColor(((r + c) % 2 == 0) ? Color(240, 217, 181) : Color(181, 136, 99));
                window.draw(square);
                Piece* p = board.getPiece({ r, c });
                if (p)
                {
                    string key = string(1, p->getColour()[0]) + "_" + p->getType();
                    Sprite sprite(textures[key]);
                    sprite.setPosition(c * 80.f, r * 80.f);
                    window.draw(sprite);
                }
            }
        if (dragging)
            window.draw(grabbed);
        window.display();
    }
    return 0;
}
