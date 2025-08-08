//  Includes
#include <stdio.h>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <glut.h>


int health = 5;          // Player starts with 5 lives
int score = 0;           // Initial score
float gameTime = 60.0f;  // Game starts with 60 seconds
const float PI = 3.14159265358979323846;
int stateOfPlayer = 0; // 0 player standing
// 1 player jumping
// 2 player ducking
float obstacleX = 1.0f;
float collectibleX = 1.0f;
float powerUp1X = 1.0f;
float powerUp2X = 1.0f;
int stateOfObstacle;
int stateOfCollectible;
int stateOfPowerUp1;
int stateOfPowerUp2;
int i = 0;
bool onScreenObstacle = false;
bool onScreenCollectible = false;
bool onScreenPowerUp1 = false;
bool onScreenPowerUp2 = false;
bool gameOn = true;
bool win = false;
bool isPowerUp1 = false;
bool isPowerUp2 = false;
float playerY = -0.5f;
float velocityY = 0.0f;
float gravity = -0.01f;
bool isJumping = false;
float playerLegsY = -0.5f;
float isDucking = false;
float incomingLow = -0.625f;
float incomingHigh = -0.3f;
float gameSpeed = 0.01f;
float cloudX1 = -1.5f;  // Initial position of the first cloud
float cloudX2 = 1.0f;   // Initial position of the second cloud
float cloudSpeed = 0.01f;  // Speed of the clouds
float collectibleY;
float yIncrement = 0.01f;  // Amount by which the y-coordinate changes each frame
bool movingUp = true;      // Direction of movement
int l = 0;

float velocityDuck = 0.0f;


//Method Signatures
//void display();
void drawPlayer();
void endGame(int x);
void obstacleArrived();
void spawnObstacle();
void displayObstacle();
void randomSpawn(int value);
void collectibleArrived();
void powerUp1Arrived();
void powerUp2Arrived();
//void drawHealthBar();
//void drawHeart();
void drawBackground() {
    glColor3f(0.6f, 0.8f, 1.0f);  // Light blue background color
    glBegin(GL_QUADS);  // Draw a simple blue background
    glVertex2f(-1.0f, -1.0f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(1.0f, 1.0f);
    glVertex2f(-1.0f, 1.0f);
    glEnd();

    // Set color for the clouds
    glColor3f(1.0f, 1.0f, 1.0f);

    // Draw first cloud (a simple circle)
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; ++i) {
        float angle = 2.0f * PI * i / 100;
        float dx = 0.15f * cos(angle);
        float dy = 0.1f * sin(angle);
        glVertex2f(cloudX1 + dx, 0.3f + dy);
    }
    glEnd();

    // Draw second cloud (another circle)
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; ++i) {
        float angle = 2.0f * PI * i / 100;
        float dx = 0.15f * cos(angle);
        float dy = 0.1f * sin(angle);
        glVertex2f(cloudX2 + dx, 0.35f + dy);
    }
    glEnd();
}
void updateBackground(int value) {
    // Move the clouds to the right
    cloudX1 += cloudSpeed;
    cloudX2 += cloudSpeed;

    // Reset cloud position when off-screen
    if (cloudX1 > 1.5f) {
        cloudX1 = -1.5f;
    }
    if (cloudX2 > 1.5f) {
        cloudX2 = -1.5f;
    }
    glutTimerFunc(100, updateBackground, 0);  // Update every 1/60th of a second
    // Request a redraw for the animation
    glutPostRedisplay();
}


void drawFilledCircle(float centerX, float centerY, float radius) {
    glBegin(GL_POLYGON); // Change to GL_POLYGON for filled circle
    for (int i = 0; i <= 100; i++) {
        float angle = 2 * PI * i / 100; // Full circle
        float x = centerX + radius * cos(angle);
        float y = centerY + radius * sin(angle);
        glVertex2f(x, y);
    }
    glEnd();
}
void drawPlus(float centerX, float centerY, float size) {
    glBegin(GL_LINES);
    // Vertical line
    glVertex2f(centerX, centerY - size / 2);
    glVertex2f(centerX, centerY + size / 2);

    // Horizontal line
    glVertex2f(centerX - size / 2, centerY);
    glVertex2f(centerX + size / 2, centerY);
    glEnd();
}
void timer(int value) {
    // Decrease time
    if (gameTime > 0) {
        gameTime -= 1.0f;
        // Update the display after 1 second (1000 ms)
        glutPostRedisplay();
        glutTimerFunc(1000, timer, 0);
    }
    else {
        printf("Game End\n");
        win = true;
        gameOn = false;
    }
}
void drawHeart() {
    // Draw the left lobe of the heart
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.0, 0.0);  // Red color for the heart
    glVertex2f(0.0f, 0.85f);  // Center of the left lobe
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.14159f * i / 100;  // Angle in radians
        float x = cos(angle) * 0.05f;  // X radius for the lobe
        float y = sin(angle) * 0.05f;  // Y radius for the lobe
        glVertex2f(-0.075f + x, 0.85f + y);
    }
    glEnd();

    // Draw the right lobe of the heart
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0.0f, 0.85f);  // Center of the right lobe
    for (int i = 0; i <= 100; i++) {
        float angle = 2.0f * 3.14159f * i / 100;
        float x = cos(angle) * 0.05f;
        float y = sin(angle) * 0.05f;
        glVertex2f(0.075f + x, 0.85f + y);
    }
    glEnd();

    // Draw the bottom part of the heart (triangle)
    glBegin(GL_TRIANGLES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-0.1f, 0.85f);   // Left corner of the triangle
    glVertex2f(0.1f, 0.85f);    // Right corner of the triangle
    glVertex2f(0.0f, 0.7f);     // Bottom point of the heart
    glEnd();
}
void drawHealthBar() {
    float x = -0.9f;
    for (int i = 0; i < health; i++) {
        glPushMatrix();
        glTranslatef(x, 0.0f, 0.0f);  // Move each heart along the x-axis
        drawHeart();                  // Draw the heart at the translated position
        glPopMatrix();
        x += 0.3f;  // Space between hearts
    }
}
void renderText(float x, float y, const std::string& text) {
    glRasterPos2f(x, y);  // Set the position for text
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);  // Render each character
    }
}
void drawScoreAndTime() {
    // Display score
    glColor3f(0.0, 1.0, 0.0);  // Green color for text
    std::stringstream ss;
    ss << "Score: " << score;
    renderText(0.6f, 0.75f, ss.str());  // Render score text

    // Display remaining time
    std::stringstream time_ss;
    time_ss << "Time: " << (int)gameTime;
    renderText(0.8f, 0.75f, time_ss.str());  // Render time text
}
void drawPlayer() {
    // 1. Head (Circle made of triangles)
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(1.0, 0.8, 0.6); // Skin color
    float x_center = -0.85f;
    float y_center = playerY + 0.15f;
    float radius = 0.05f;
    int num_segments = 20; // Increase for smoother circle
    for (int i = 0; i <= num_segments; i++) {
        float angle = 2.0f * 3.14159f * i / num_segments;
        float x = radius * cos(angle);
        float y = radius * sin(angle);
        glVertex2f(x_center + x, y_center + y);
    }
    glEnd();

    // 2. Body (Line strip)
    glBegin(GL_QUADS);
    glColor3f(0.0, 0.0, 1.0); // Blue color for the body
    glVertex2f(-0.9f, playerY + 0.1f);  // Top-left corner
    glVertex2f(-0.9f, playerY - 0.1f); // Bottom-left corner
    glVertex2f(-0.8f, playerY + 0.1f);  // Top-right corner
    glVertex2f(-0.8f, playerY - 0.1f); // Bottom-right corner
    glEnd();
    //neck
    glBegin(GL_LINES);
    glVertex2f(-0.85f, playerY + 0.1f);
    glVertex2f(-0.85f, playerY - 0.1f);
    glEnd();
    // 3. Arms (Line strips)
    glBegin(GL_LINES);
    glVertex2f(-0.95f, playerY); // Left arm from the body
    glVertex2f(-0.75f, playerY);  // Right arm from the body
    glEnd();

    // 4. Legs (Line strips)
    glBegin(GL_LINES);
    glVertex2f(-0.85f, playerY - 0.1f); // Left leg from the waist
    glVertex2f(-0.9f, playerLegsY - 0.2f);
    glVertex2f(-0.85f, playerY - 0.1f); // Right leg from the waist
    glVertex2f(-0.8f, playerLegsY - 0.2f);
    glEnd();
}
void updateObstaclePosition(int value) {
    if (!onScreenObstacle) {
        return;
    }
    obstacleX -= gameSpeed; // Move the obstacle to the left

    if (obstacleX < -0.8f) {
        obstacleArrived();
        obstacleX = 1.0f;
        onScreenObstacle = false;
        gameSpeed += 0.0005f;
        randomSpawn(0);
    }

    glutPostRedisplay(); // Redraw the scene with the updated position
}
void updateCollectiblePosition(int value) {
    if (!onScreenCollectible) {
        return;
    }
    if (stateOfCollectible == 1) {

    

        if (movingUp) {
            collectibleY += yIncrement; // Move up
            if (collectibleY >= -0.25f) { // Upper limit
                movingUp = false;       // Start moving down
            }
        }
        else {
            collectibleY -= yIncrement; // Move down
            if (collectibleY <= -0.35f) { // Lower limit
                movingUp = true;        // Start moving up
            }
        }
    }
    else if (stateOfCollectible == 0) {
        if (movingUp) {
            collectibleY += yIncrement; // Move up
            if (collectibleY >= -0.575) { // Upper limit
                movingUp = false;       // Start moving down
            }
        }
        else {
            collectibleY -= yIncrement; // Move down
            if (collectibleY <= -0.675f) { // Lower limit
                movingUp = true;        // Start moving up
            }
        }
    }
    collectibleX -= gameSpeed; // Move the obstacle to the left

    if (collectibleX < -0.8f) {
        collectibleArrived();
        collectibleX = 1.0f;
        onScreenCollectible = false;
        gameSpeed += 0.0005f;
        randomSpawn(0);
    }

    glutPostRedisplay(); // Redraw the scene with the updated position
}
void updatePowerUp1Position(int value) {
    if (!onScreenPowerUp1) {
        return;
    }
    powerUp1X -= gameSpeed; // Move the obstacle to the left

    if (powerUp1X < -0.8f) {
        powerUp1Arrived();
        powerUp1X = 1.0f;
        onScreenPowerUp1 = false;
        gameSpeed += 0.0005f;
        randomSpawn(0);
    }

    glutPostRedisplay(); // Redraw the scene with the updated position
}
void updatePowerUp2Position(int value) {
    if (!onScreenPowerUp2) {
        return;
    }
    powerUp2X -= gameSpeed; // Move the obstacle to the left

    if (powerUp2X < -0.8f) {
        powerUp2Arrived();
        powerUp2X = 1.0f;
        onScreenPowerUp2 = false;
        gameSpeed += 0.0005f;
        randomSpawn(0);
    }

    glutPostRedisplay(); // Redraw the scene with the updated position
}
void drawCollectible(float x, float y, float radius) {
    // Set color to yellow for the collectible
    glColor3f(1.0f, 1.0f, 0.0f);

    // 1. Draw a filled circle (using triangle fan to approximate the circle)
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y); // Center of the circle
    for (int i = 0; i <= 100; ++i) {
        float angle = i * 2.0f * PI / 100;
        float dx = radius * cos(angle);
        float dy = radius * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();

    // Set color to orange for decoration
    glColor3f(1.0f, 0.5f, 0.0f);

    // 2. Draw a square inside the circle (GL_QUADS)
    float innerRadius = radius * 0.5f;
    glBegin(GL_QUADS);
    glVertex2f(x - innerRadius, y - innerRadius); // Bottom-left
    glVertex2f(x + innerRadius, y - innerRadius); // Bottom-right
    glVertex2f(x + innerRadius, y + innerRadius); // Top-right
    glVertex2f(x - innerRadius, y + innerRadius); // Top-left
    glEnd();

    // Set color to white for highlights
    glColor3f(1.0f, 1.0f, 1.0f);

    // 3. Draw a small triangle on top (GL_TRIANGLES)
    glBegin(GL_TRIANGLES);
    glVertex2f(x, y + radius);            // Top of the triangle
    glVertex2f(x - radius * 0.3f, y + radius * 0.6f); // Bottom-left
    glVertex2f(x + radius * 0.3f, y + radius * 0.6f); // Bottom-right
    glEnd();

    // Set color to a darker yellow for contrast
    glColor3f(0.8f, 0.8f, 0.0f);

    // 4. Draw a hexagon inside the circle (GL_POLYGON)
    glBegin(GL_POLYGON);
    for (int i = 0; i < 6; ++i) {
        float angle = i * 2.0f * PI / 6;
        float dx = innerRadius * 0.8f * cos(angle);
        float dy = innerRadius * 0.8f * sin(angle);
        glVertex2f(x + dx, y + dy);
    }
    glEnd();
}
void displayCollectible() {

    if (stateOfCollectible == 0) {
        glColor3f(0.0f, 1.0f, 0.0f); // Green for the circle
        drawCollectible(collectibleX, collectibleY, 0.05f); // Draw circle at origin with radius 0.5
    }
    else if (stateOfCollectible == 1) {
        glColor3f(0.0f, 1.0f, 0.0f); // Green for the circle
        drawCollectible(collectibleX, collectibleY, 0.05f); // Draw circle at origin with radius 0.5

    }
}
void drawTerrain() {
    glColor3f(0.4f, 0.8f, 0.4f); // Green color for the terrain

    // First section: a flat area (Quad)
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, -1.0f);  // Bottom-left
    glVertex2f(-0.6f, -1.0f);  // Bottom-right
    glVertex2f(-0.6f, -0.7f);  // Top-right
    glVertex2f(-1.0f, -0.7f);  // Top-left
    glEnd();

    // Second section: a hill (Triangle)
    glColor3f(0.3f, 0.7f, 0.3f);  // Darker green for variation
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.6f, -1.0f);  // Bottom-left
    glVertex2f(-0.3f, -1.0f);  // Bottom-right
    glVertex2f(-0.45f, -0.7f); // Top (peak of the hill)
    glEnd();

    // Third section: another flat area (Quad)
    glColor3f(0.4f, 0.8f, 0.4f);  // Back to green
    glBegin(GL_QUADS);
    glVertex2f(-0.3f, -1.0f);  // Bottom-left
    glVertex2f(0.2f, -1.0f);   // Bottom-right
    glVertex2f(0.2f, -0.7f);   // Top-right
    glVertex2f(-0.3f, -0.7f);  // Top-left
    glEnd();

    // Fourth section: another hill (Triangle)
    glColor3f(0.3f, 0.7f, 0.3f);  // Darker green again
    glBegin(GL_TRIANGLES);
    glVertex2f(0.2f, -1.0f);   // Bottom-left
    glVertex2f(0.6f, -1.0f);   // Bottom-right
    glVertex2f(0.4f, -0.7f);   // Top (peak of the hill)
    glEnd();

    // Fifth section: another flat area (Quad)
    glColor3f(0.4f, 0.8f, 0.4f);  // Green color for the final flat area
    glBegin(GL_QUADS);
    glVertex2f(0.6f, -1.0f);   // Bottom-left
    glVertex2f(1.0f, -1.0f);   // Bottom-right
    glVertex2f(1.0f, -0.7f);   // Top-right
    glVertex2f(0.6f, -0.7f);   // Top-left
    glEnd();
}
void drawPowerUp1(float x, float y) {
    float height = 0.1; // Height of the power-up

    // Rectangle body
    glColor3f(1.0, 0.5, 0.0); // Orange color
    glBegin(GL_QUADS);
    glVertex2f(x - 0.05, y);        // Bottom-left
    glVertex2f(x + 0.05, y);        // Bottom-right
    glVertex2f(x + 0.05, y + height); // Top-right
    glVertex2f(x - 0.05, y + height); // Top-left
    glEnd();

    // Triangle on top
    glColor3f(0.0, 0.5, 1.0); // Blue color
    glBegin(GL_TRIANGLES);
    glVertex2f(x - 0.03, y + height);  // Bottom-left
    glVertex2f(x + 0.03, y + height);  // Bottom-right
    glVertex2f(x, y + height + 0.05);  // Top
    glEnd();

    // Left circle (using a triangle fan)
    glColor3f(1.0, 0.0, 0.0); // Red color
    float radius = 0.03;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x - 0.04, y + 0.05); // Center of circle
    for (int i = 0; i <= 20; ++i) {
        float theta = i * 2.0f * 3.14159f / 20;  // Angle
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x - 0.04 + dx, y + 0.05 + dy);
    }
    glEnd();

    // Right circle (using a triangle fan)
    glColor3f(1.0, 0.0, 0.0); // Red color
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x + 0.04, y + 0.05); // Center of circle
    for (int i = 0; i <= 20; ++i) {
        float theta = i * 2.0f * 3.14159f / 20;  // Angle
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + 0.04 + dx, y + 0.05 + dy);
    }
    glEnd();
}
void drawPowerUp2(float x, float y) {
    float height = 0.1; // Height of the power-up

    // Hexagon body (using triangle fan)
    glColor3f(0.0, 1.0, 0.0); // Green color
    float radius = 0.05;
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(x, y + 0.05); // Center of hexagon
    for (int i = 0; i <= 6; ++i) {
        float theta = i * 2.0f * 3.14159f / 6;  // Angle for hexagon
        float dx = radius * cosf(theta);
        float dy = radius * sinf(theta);
        glVertex2f(x + dx, y + 0.05 + dy);
    }
    glEnd();

    // Star on top (5 points)
    glColor3f(1.0, 1.0, 0.0); // Yellow color
    glBegin(GL_TRIANGLES);
    for (int i = 0; i < 5; ++i) {
        float angle1 = i * 2.0f * 3.14159f / 5;
        float angle2 = (i + 1) * 2.0f * 3.14159f / 5;
        glVertex2f(x, y + height + 0.05);
        glVertex2f(x + 0.04 * cosf(angle1), y + height + 0.05 + 0.04 * sinf(angle1));
        glVertex2f(x + 0.04 * cosf(angle2), y + height + 0.05 + 0.04 * sinf(angle2));
    }
    glEnd();

    // Vertical line
    glColor3f(1.0, 0.0, 1.0); // Magenta color
    glBegin(GL_LINES);
    glVertex2f(x, y);           // Bottom
    glVertex2f(x, y + height);  // Top
    glEnd();

    // Horizontal line
    glColor3f(1.0, 0.0, 1.0); // Magenta color
    glBegin(GL_LINES);
    glVertex2f(x - 0.05, y + 0.05); // Left
    glVertex2f(x + 0.05, y + 0.05); // Right
    glEnd();
}
void displayPowerUp1() {
    if (stateOfPowerUp1 == 0) {
        glColor3f(0.0f, 0.0f, 1.0f); // Green for the circle
        drawPowerUp1(powerUp1X, incomingLow); // Draw circle at origin with radius 0.5
    }
    else if (stateOfPowerUp1 == 1) {
        glColor3f(0.0f, 0.0f, 1.0f); // Green for the circle
        drawPowerUp1(powerUp1X, incomingHigh); // Draw circle at origin with radius 0.5

    }
}
void displayPowerUp2() {
    if (stateOfPowerUp2 == 0) {
        glColor3f(1.0f, 0.0f, 0.0f); // Green for the circle
        drawPowerUp2(powerUp2X, incomingLow); // Draw circle at origin with radius 0.5
    }
    else if (stateOfPowerUp2 == 1) {
        glColor3f(1.0f, 0.0f, 0.0f); // Green for the circle
        drawPowerUp2(powerUp2X, incomingHigh); // Draw circle at origin with radius 0.5
    }
}
void displayObstacle() {

    if (stateOfObstacle == 0) {
        glColor3f(1.0f, 0.0f, 0.0f); // Red for the circle
        drawFilledCircle(obstacleX, incomingLow, 0.05f); // Draw circle at origin with radius 0.5
        // Set color for the plus symbol
        glColor3f(0.0f, 0.0f, 1.0f); // Blue for the plus
        drawPlus(obstacleX, incomingLow, 0.15f); // Draw plus at origin with size 0.3

    }
    else if (stateOfObstacle == 1) {

        glColor3f(1.0f, 0.0f, 0.0f); // Red for the circle
        drawFilledCircle(obstacleX, incomingHigh, 0.05f); // Draw circle at origin with radius 0.5
        // Set color for the plus symbol
        glColor3f(0.0f, 0.0f, 1.0f); // Blue for the plus
        drawPlus(obstacleX, incomingHigh, 0.15f); // Draw plus at origin with size 0.3
    }

    glutSwapBuffers();
}
void endGame(int x) {
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    glLoadIdentity();             // Reset the transformation matrix

    if (x == 0) {
        glColor3f(1.0, 0.0, 0.0);  // Green color for text
        std::stringstream ss;
        ss << "Game Lose ";
        renderText(0.0f, 0.0f, ss.str());
    }
    else {
        glColor3f(0.0, 1.0, 0.0);  // Green color for text
        std::stringstream ss;
        ss << "Game End";
        renderText(0.0f, 0.0f, ss.str());
    }
    glColor3f(0.0, 1.0, 0.0);  // Green color for text
    std::stringstream ss;
    ss << "Score: " << score;
    renderText(0.6f, 0.75f, ss.str());  // Render score text
    glFlush();  // Render now


}
void onCollisionWithObstacle() {
    if (isPowerUp2) {
        return;
    }
    health--;
    if (health == 0) {
        printf("Game Lose\n");
        win = false;
        gameOn = false;
    }
    onScreenObstacle = false;
    onScreenCollectible = false;
    onScreenPowerUp1 = false;
    onScreenPowerUp2 = false;
    obstacleX = 1.0f;
    collectibleX = 1.0f;
    powerUp1X = 1.0f;
    powerUp2X = 1.0f;
	gameSpeed = 0.01f;


}
void obstacleArrived() {

    if (stateOfPlayer == 0) {
        onCollisionWithObstacle();
    }
    else if (stateOfPlayer == 2 && stateOfObstacle == 0) {
        onCollisionWithObstacle();
    }
    else if (stateOfPlayer == 1 && stateOfObstacle == 1) {
        onCollisionWithObstacle();
    }
}
void onCollisionWithCollectible() {
    // add score
    if (isPowerUp1) {
        score = score + 200;
    }
    else {
        score = score + 100;
    }
}
void deactivatePowerUp1(int value) {
    isPowerUp1 = false;
    std::cout << "PowerUp1 deactivated after 10 seconds." << std::endl;
}
void deactivatePowerUp2(int value) {
    isPowerUp2 = false;
    std::cout << "PowerUp2 deactivated after 10 seconds." << std::endl;
}
void onCollisionWithPowerUp1() {
    isPowerUp1 = true;
    glutTimerFunc(5000, deactivatePowerUp1, 0);
}
void onCollisionWithPowerUp2() {
    isPowerUp2 = true;
    glutTimerFunc(5000, deactivatePowerUp2, 0);
}
void powerUp1Arrived() {

    if (stateOfPlayer == 0) {
        onCollisionWithPowerUp1();
    }
    else if (stateOfPlayer == 2 && stateOfPowerUp1 == 0) {
        onCollisionWithPowerUp1();
    }
    else if (stateOfPlayer == 1 && stateOfPowerUp1 == 1) {
        onCollisionWithPowerUp1();
    }
}
void powerUp2Arrived() {
    if (stateOfPlayer == 0) {
        onCollisionWithPowerUp2();
    }
    else if (stateOfPlayer == 2 && stateOfPowerUp2 == 0) {
        onCollisionWithPowerUp2();
    }
    else if (stateOfPlayer == 1 && stateOfPowerUp2 == 1) {
        onCollisionWithPowerUp2();
    }
}
void collectibleArrived() {

    if (stateOfPlayer == 0) {
        onCollisionWithCollectible();
    }
    else if (stateOfPlayer == 2 && stateOfCollectible == 0) {
        onCollisionWithCollectible();
    }
    else if (stateOfPlayer == 1 && stateOfCollectible == 1) {
        onCollisionWithCollectible();
    }
}
void Jump() {
    if (isJumping) {
        // Update y position based on velocity
        playerY += velocityY;
        playerLegsY += velocityY;

        // Decrease velocity to simulate gravity pulling the player down
        velocityY += gravity;

        // Check if player has landed
        if (playerY <= -0.5f) {
            // Player has landed, reset position and velocity
            playerY = -0.5f;
            playerLegsY = -0.5f;
            velocityY = 0.0f;
            isJumping = false;  // Jumping is complete
            stateOfPlayer = 0;
        }
    }
    //playerY = 0.0f;
 //   stateOfPlayer = 1;
}
void startJump() {
    if (!isJumping) {
        // Player is not already jumping, initiate jump
        isJumping = true;
        stateOfPlayer = 1;
        velocityY = 0.1f;  // Set an initial upward velocity
    }
}
void Duck() {
    if (isDucking) {
        // Update y position based on velocity
        playerY -= velocityDuck;


        velocityDuck += -0.0005f;

        if (playerY == -0.6f) {
            velocityDuck = 0.0f;
        }
        if (playerY >= -0.5f) {
            playerY = -0.5f;
            velocityDuck = 0.0f;
            isDucking = false;
            stateOfPlayer = 0;
        }
    }

    //playerY = -0.5f;
 //   stateOfPlayer = 2;
}
void startDuck() {
    isDucking = true;
    stateOfPlayer = 2;
    velocityDuck = 0.01f;
}
void KeyUp(unsigned char key, int x, int y) {
    if (key == 'w') {
        startJump();
    }
    else if (key == 's') {

        startDuck();
    }
}
void spawnObstacle() {
    srand(time(0));

    stateOfObstacle = (rand() % 2);
    onScreenObstacle = true;
    displayObstacle();
}
void spawnCollectible() {
    srand(time(0));
    onScreenCollectible = true;
    stateOfCollectible = (rand() % 2);
    if (stateOfCollectible == 0) {
		collectibleY = incomingLow;
	}
    else if (stateOfCollectible == 1) {
		collectibleY = incomingHigh;
    }
    displayCollectible();
}
void spawnPowerUp1() {
    srand(time(0));
    onScreenPowerUp1 = true;
    stateOfPowerUp1 = (rand() % 2);
    displayPowerUp1();
}
void spawnPowerUp2() {
    srand(time(0));
    onScreenPowerUp2 = true;
    stateOfPowerUp2 = (rand() % 2);
    displayPowerUp2();
}
void spawnPowerUp() {
    srand(time(0));
    int randomNum = (rand() % 2) + 1;
    if (randomNum == 1) {
        spawnPowerUp1();
    }
    else if (randomNum == 2) {
        spawnPowerUp2();
    }
}
void randomSpawn(int value) {
    srand(time(0));
    bool flag = false;
    while (!flag) {
        int randomNum = (rand() % 5) + 1;
        if ((randomNum == 1 || randomNum == 4) && onScreenObstacle == false) {
            flag = true;
            spawnObstacle();
        }
        else if ((randomNum == 2 || randomNum == 5) && onScreenCollectible == false) {
            flag = true;
            spawnCollectible();
        }
        else if (randomNum == 3 && onScreenPowerUp1 == false && onScreenPowerUp2 == false) {
            flag = true;
            spawnPowerUp();
        }

    }



}
void drawUpperBoundary() {
    // Set line width for better visibility
    glLineWidth(3.0f);
    // 1. Draw a line representing the upper boundary
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.7f);  // Left end
    glVertex2f(1.0f, 0.7f);   // Right end
    glEnd();

    // 2. Draw a triangle as decoration
    float triangleX = 0.95f;
    while (triangleX > -1.0f) {
        glBegin(GL_TRIANGLES);
        glVertex2f(triangleX, 0.7f);  // Top of the triangle
        glVertex2f(triangleX - 0.05f, 0.6f); // Bottom left corner
        glVertex2f(triangleX + 0.05, 0.6f); // Bottom right corner
        glEnd();
        triangleX -= 0.06f;
    }
    glBegin(GL_QUADS);
    glVertex2f(-1.0f, 0.7f);  // Top left
    glVertex2f(-1.0f, 0.68f); // Bottom left
    glVertex2f(1.0f, 0.68f);  // Bottom right
    glVertex2f(1.0f, 0.7f);   // Top right
    glEnd();
}
void display() {

    if (!gameOn) {
        if (win) {
            endGame(1);
        }
        else {
            endGame(0);
        }
        return;
    }
    glClear(GL_COLOR_BUFFER_BIT); // Clear the screen
    glLoadIdentity();             // Reset the transformation matrix
    drawBackground();

    drawTerrain();
    drawUpperBoundary();
    glPushMatrix();
    glScalef(0.5f, 0.5f, 0.5f);
    drawPlayer();
    glPopMatrix();

    drawHealthBar();
    drawScoreAndTime();
    Jump();
    Duck();

    if (i == 0) {
        randomSpawn(0);
        i++;
    }
    if (onScreenObstacle) {
        displayObstacle();
        glutTimerFunc(100, updateObstaclePosition, 0);
    }
    if (onScreenCollectible) {
        displayCollectible();
        glutTimerFunc(100, updateCollectiblePosition, 0);

    }
    if (onScreenPowerUp1) {
        if (l < 10) {
            glPushMatrix();
            glScalef(0.95f, 0.95f, 0.95f);
            //glRotatef(45.0f,0.0f,1.0f,0.0f);
            displayPowerUp1();
            glPopMatrix();
            l++;
        }
        else{
            glPushMatrix();
            glScalef(1.05f, 1.05f, 1.05f);
            //glRotatef(45.0f,0.0f,1.0f,0.0f);
            displayPowerUp1();
            glPopMatrix();
            l++;
            if (l == 20) {
                l = 0;
            }
        }

        
        
        glutTimerFunc(100, updatePowerUp1Position, 0);
    }
    if (onScreenPowerUp2) {
        displayPowerUp2();
        glutTimerFunc(100, updatePowerUp2Position, 0);
    }

    glFlush();

}

// Main function
void main(int argc, char** argr) {
    glutInit(&argc, argr);
    glutInitWindowSize(1500, 800);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("2D Player - Infinite Runner Game");
    glutDisplayFunc(display); // Set display callback function
    glutKeyboardUpFunc(KeyUp);
    glutTimerFunc(1000, timer, 0);  // Start timer for countdown
    glutTimerFunc(5000, randomSpawn, 0);
    glutTimerFunc(1000, updateBackground, 0);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glClearColor(1, 1, 1, 0);
    gluOrtho2D(0, 800, 0, 800);
    glutMainLoop(); // Enter main event loop
}