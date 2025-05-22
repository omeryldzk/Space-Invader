#include "Player.h"
#include "Alien.h" 
#include "Bullet.h"
#include "Game.h"
#include "Position.h"
#include "Constants.h"
#include <vector>
#include <string>   
#include <iostream>
#include <cstdlib>  
#include <ctime>    
#include <thread>   
#include <chrono>  

Game::Game()
    : player(BOARD_WIDTH / 2, PLAYER_START_Y, INITIAL_LIVES),
      score(0),
      gameOverState(false),
      alienDirection(1), // 1 for right, -1 for left
      aliensNeedToShiftDown(false),
      gameSpeed(1)
      {

    // Initialize the game board
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            gameBoard[y][x] = ' ';
        }
    }

    // Initialize aliens
    for (int row = 0; row < ALIEN_ROWS; ++row) {
        for (int col = 0; col < ALIENS_PER_ROW; ++col) {
            if(row == ALIEN_ROWS - 1){
                int startX = col * ALIEN_SPACING_X;
                int startY = ALIEN_START_ROW + row;
                aliens.emplace_back(startX, startY, true); // Last row can shoot
            }
            else{
                int startX = col * ALIEN_SPACING_X;
                int startY = ALIEN_START_ROW + row;
                aliens.emplace_back(startX, startY); // Other rows cannot shoot
            }
        }
    }

    // Initialize random seed
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

void Game::run() {
    std::cout << "--------------------------------" << std::endl;
    std::cout << "Press 'a' to move left, 'd' to move right, 's' to shoot." << std::endl;
    std::cout << "--------------------------------" << std::endl;
    int round = 0;
    while (!gameOverState) {
        // her 10 harekette bir hızlanır
        if(round % 10 == 0){
            gameSpeed++;
            round = 0;
        }
        // Clear the game board
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                gameBoard[y][x] = ' ';
            }
        }
        

        // Alienlar gameSpeede göre hareket eder 
        // Duvara çarparsa aşağıya kaydırır
        moveAliens();

        // Alien mermisi playerı vurursa playerın canını azaltır
        // Alien mermisi playerı vurursa ya da board dışına çıkarsa alienin mermisini sıfırlar
        // Player mermisi alieni vurursa alieni öldürür
        // Player mermisi alieni vurursa ya da board dışına çıkarsa playerın mermisini sıfırlar
        // Alien eğer ölürse üstündeki alieni can shoot yapar
        checkCollisions();
        

        // Canlı olan herşeyi çizer
        drawGameBoard();

        // Userdan input alır
        handleInput();

        // Player ve alien mermilerini günceller
        // Alienın önünde alien yoksa %5 ihtimalle mermi atar
        playerAliensShoot();
        
        system("clear"); 

        round++;

        // Playerın canı bitince oyunu bitirir
        // Tüm alienler öldüğünde oyunu bitirir
        // Alienler en alttaki satıra gelirse oyunu bitirir
        checkGameOver();
    }
    bool allAliensDead = true;
    for(auto& alien : aliens){
        if(alien.isAlive()){
            std::cout << "You lose! Your score: " << score << std::endl;
            allAliensDead = false;
            break;
        }
    }
    if(allAliensDead)
        std::cout << "You win! Your score: " << score << std::endl;
    }


void Game::moveAliens() {
    // Move aliens based on the current direction
    for (auto& alien : aliens) {
        if (alien.isAlive()) {
            alien.move(alienDirection * gameSpeed, 0);
        }
        if (alien.isAlive() && (alien.getPosition().x >= BOARD_WIDTH - gameSpeed || alien.getPosition().x <=  gameSpeed - 1)) {
            aliensNeedToShiftDown = true;
        }
    }
    // Check if aliens need to shift down
    if (aliensNeedToShiftDown) {
        for (auto& alien : aliens) {
            if (alien.isAlive()) {
                alien.move(0, 1); // Move down
            }
        }
        alienDirection *= -1; // Change direction
        aliensNeedToShiftDown = false;
    }
}

// Finds the alien with the maximum y coordinate in the same column
// Returns -1 if not found
void Game::findAlienIndex(int x) {
    int maxY = -1;
    int index = -1;
    for (size_t i = 0; i < aliens.size(); ++i) {
        if (aliens[i].isAlive() && aliens[i].getPosition().x == x)
        {
            if(aliens[i].getPosition().y > maxY){
                maxY = aliens[i].getPosition().y;
                index = i;
            }
        }
    }
    if(index != -1){
        aliens[index].setCanShoot(true); // Set the alien to shoot
    }
}



void Game::checkCollisions() {
    int indexY, indexX;
    for (auto& alien : aliens) {
        // Check for player bullet collisions with aliens
        if (alien.isAlive() && player.getBullet().isActive() && alien.getPosition() == player.getBullet().getPosition()) {
            alien.destroy();
            player.resetBullet(); // Reset the player's bullet
            score += 10; // Increase score
            findAlienIndex(player.getBullet().getPosition().x); // Finds if there is an alien in the same column and sets it to Canshoot
        }
        // Check if alien bullet collides with the player
        if (alien.alienBullet.isActive() && alien.alienBullet.getPosition() == player.getPosition()) {
            player.takeDamage();
            if(alien.isAlive()){
                alien.resetBullet(); // Deactivate the bullet after collision
            }
        }

        // Check if the alien bullet is out of bounds
        if (alien.alienBullet.isActive() && (alien.alienBullet.getPosition().y >= BOARD_HEIGHT)) {
            alien.resetBullet(); // Deactivate the bullet if out of bounds
        }
    }
    // Check if the player bullet is out of bounds
    if (player.getBullet().isActive() && (player.getBullet().getPosition().y < 0)) {
        player.resetBullet(); // Deactivate the bullet if out of bounds
    }
}

void Game::handleInput() {
    // Handle user input (left, right, shoot)
    char input;

    std::cin >> input;

    switch (input) {
        case 'a': 
            // Check if the player can move left
            if (player.getPosition().x > 0) {
                player.moveLeft();
            }
            break;
        case 'd': 
            // Check if the player can move right
            if (player.getPosition().x < BOARD_WIDTH - 1) {
                player.moveRight();
            }
            break;
        case 's': // Shoot
            player.shoot();
            break;
        default:
            break;
    }
}

void Game::playerAliensShoot() {
    // Player shoots
    if (player.getBullet().isActive()) {
        player.updateBullet();
    }

    for (auto& alien : aliens) {
        // If the alien is alive and dont have active bullet, try to shoot
        if (alien.isAlive() && !alien.alienBullet.isActive()) {
            alien.tryToShoot();
        }
        // If the alien bullet is active, update it
        if (alien.alienBullet.isActive()) {
            alien.updateBullet();
        } 
    }
}

void Game::drawGameBoard() {
    // Draw the game board
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            gameBoard[y][x] = ' ';
        }
    }

    // Draw player
    gameBoard[player.getPosition().y][player.getPosition().x] = player.getSymbol();

    // Draw player bullet
    if (player.getBullet().isActive()) {
        gameBoard[player.getBullet().getPosition().y][player.getBullet().getPosition().x] = player.getBullet().getSymbol();
    }

    // Draw aliens and bullets
    for (const auto& alien : aliens) {
        if (alien.isAlive()) {
            gameBoard[alien.getPosition().y][alien.getPosition().x] = alien.getSymbol();
        }
        if (alien.alienBullet.isActive()) {
            gameBoard[alien.alienBullet.getPosition().y][alien.alienBullet.getPosition().x] = alien.alienBullet.getSymbol();
        }
    }

    // Print the game board
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            std::cout << gameBoard[y][x];
        }
        std::cout << std::endl;
    }

    // Print score and lives
    std::cout << "Score: " << score << " Lives: " << player.lives << std::endl;
}

void Game::checkGameOver() {
    if (!player.isAlive()) {
        gameOverState = true;
    }
    // Check if all aliens are dead 
    bool allAliensDead = true;
    for (const auto& alien : aliens) {
        if (alien.isAlive()) {
            allAliensDead = false;
        }
        if (alien.getPosition().y >= BOARD_HEIGHT - 1) {
            gameOverState = true; // Game over if any alien reaches the bottom
        }
    }
    if (allAliensDead) {
        gameOverState = true;
    }
}

