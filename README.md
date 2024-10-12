# Chess Game with SFML

## Table of Contents

1. [Overview](#overview)
2. [Features](#features)
3. [Game Mechanics](#game-mechanics)
4. [Installation](#installation)
5. [Running the Game](#running-the-game)
6. [Project Structure](#project-structure)
7. [Usage](#usage)
8. [Known Issues](#known-issues)
9. [Future Enhancements](#future-enhancements)
10. [Credits](#credits)

## Overview

This project is a simple chess game implemented in C++ using the **SFML (Simple and Fast Multimedia Library)**. The game features a fully rendered chessboard and pieces, enabling two players to play a standard game of chess. Players can interact with the board using their mouse to drag and drop pieces, and the game enforces standard chess rules such as legal movement patterns for each type of piece.

The goal of this project is to provide a basic yet functional chess game, suitable for educational purposes or for those who want to practice building games with SFML.

## Features

- **Chessboard Rendering:** The game renders an 8x8 chessboard and all the pieces at the start of the game.
- **Drag-and-Drop Movement:** Players can drag and drop pieces to make moves.
- **Standard Chess Rules:** The game prevents illegal moves based on standard chess rules (e.g., pawns moving diagonally to attack, knights moving in "L" shapes, etc.).
- **Piece Classes:** Different piece types (e.g., Queen, Knight, Bishop, etc.) are represented as distinct C++ classes, each with their own movement logic.
- **Move Validation:** Basic implementation of chess rules to restrict illegal moves (e.g., pieces can’t jump over others, check is considered).

## Game Mechanics

- **Piece Movement:** Each piece moves according to standard chess rules:
  - Pawns move one square forward (or two on their first move) and capture diagonally.
  - Rooks move horizontally or vertically.
  - Bishops move diagonally.
  - Knights move in an "L" shape.
  - Queens can move in any direction.
  - Kings move one square in any direction.
  
- **Castling:** Not implemented yet.
- **En Passant:** Not implemented yet.
- **Check and Checkmate:** In development.
- **Promotion:** When a pawn reaches the last rank, it is promoted to a Queen (other promotions not yet implemented).

## Installation

### Prerequisites

Before running the chess game, make sure you have the following dependencies installed:

- A modern **C++ compiler** (g++ or clang++)
- **SFML** version 2.5.1 or higher

To install SFML on Ubuntu, you can use:

```bash
sudo apt-get install libsfml-dev
