♟️ C++ Chess Game
This is a Chess Game with SFML developed in C++ using Object-Oriented Programming (OOP) principles as a course project. The game allows two players to play standard chess in a turn-based manner.

Team Members:
Wajiha -> Leader

Misha

Gotam


Features:
Full 8x8 chess board representation

All standard chess pieces (Pawn, Rook, Knight, Bishop, Queen, King)

Proper movement rules for each piece

Turn-based two-player gameplay

Object-Oriented Design:

Inheritance for piece hierarchy

Polymorphism to handle piece movement

Encapsulation for data safety

Operator Overloading where appropriate (e.g., for position comparison)

Project Structure

/chess-game/
│
├── main.cpp           # Entry point of the game
├── ChessBoard.h/.cpp  # Chess board logic
├── Piece.h/.cpp       # Abstract base class for pieces
├── Pawn.h/.cpp        # Derived class for Pawn
├── Rook.h/.cpp        # Derived class for Rook
├── ...                # Other piece classes (Knight, Bishop, Queen, King)
├── Player.h/.cpp      # Player class
└── README.md          # Project documentation

🎮 How to Play

Clone the repository:
git clone https://github.com/your-username/chess-game.git

Compile the code using any C++ compiler:
g++ main.cpp -o ChessGame

Run the game:
./ChessGame

Players take turns entering the coordinates for moving their pieces. The game validates each move and enforces the standard rules of chess.

✅ OOP Concepts Used
Inheritance: All chess pieces inherit from a base Piece class.

Polymorphism: Each piece class overrides a canMove() function to implement movement rules.

Encapsulation: Board state and piece information are kept private.

Operator Overloading: Used for comparing board positions and possibly printing board states.

🚧 Future Improvements
Implement check and checkmate detection

Add castling, en passant, and pawn promotion

Save/load game feature
