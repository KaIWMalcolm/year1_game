#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

const Keyboard::Key controls[4] = {
    Keyboard::A, 
    Keyboard::Z, 
    Keyboard::O,  
    Keyboard::L 
};
const Vector2f paddleSize(25.f, 100.f);
const float ballRadius = 10.f;
const int gameWidth = 800;
const int gameHeight = 600;
const float paddleSpeed = 400.f;
const float paddleOffsetWall = 10.f;
Vector2f ballVelocity;
bool isPlayer1Serving = false;
const float initialVelocityX = 100.f;
const float initialVelocityY = 60.f;
const float velocityMultiplier = 1.1f;

CircleShape ball;
RectangleShape paddles[2];

void Load() {
    for (auto &p : paddles) {
        p.setSize(paddleSize);
        p.setOrigin(paddleSize / 2.f);
    }
    ball.setRadius(ballRadius);
    ball.setOrigin(ballRadius, ballRadius);

    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));
 
    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}
void Reset() {
    ball.setOrigin(ballRadius, ballRadius);

    paddles[0].setPosition(Vector2f(paddleOffsetWall + paddleSize.x / 2.f, gameHeight / 2.f));
    paddles[1].setPosition(Vector2f(gameWidth - paddleOffsetWall - paddleSize.x / 2.f, gameHeight / 2.f));

    ball.setPosition(Vector2f(gameWidth / 2.f, gameHeight / 2.f));
    ballVelocity = { (isPlayer1Serving ? initialVelocityX : -initialVelocityX), initialVelocityY };
}

void Update(RenderWindow &window) {
    // Reset clock, recalculate deltatime
    static Clock clock;
    float dt = clock.restart().asSeconds();
    // check and consume events
    Event event;
    while (window.pollEvent(event)) {
        if (event.type == Event::Closed) {
            window.close();
            return;
        }
    }

    ball.move(ballVelocity * dt);

    // Quit Via ESC Key
    if (Keyboard::isKeyPressed(Keyboard::Escape)) {
        window.close();
    }

    // handle paddle movement
    float direction1 = 0.0f;
    if (Keyboard::isKeyPressed(controls[0])) {
        direction1--;
    }
    if (Keyboard::isKeyPressed(controls[1])) {
        direction1++;
    }
    paddles[0].move(Vector2f(0.f, direction1 * paddleSpeed * dt));

    // Check for out of bounds (Player 1)
    float py1 = paddles[0].getPosition().y;
    if (py1 + paddleSize.y / 2 > gameHeight) {  // bottom boundary
        paddles[0].setPosition(paddles[0].getPosition().x, gameHeight - paddleSize.y / 2);
    }
    else if (py1 - paddleSize.y / 2 < 0) {  // top boundary
        paddles[0].setPosition(paddles[0].getPosition().x, paddleSize.y / 2);
    }

    float direction2 = 0.0f;
    if (Keyboard::isKeyPressed(controls[2])) {
        direction2--;
    }
    if (Keyboard::isKeyPressed(controls[3])) {
        direction2++;
    }
    paddles[1].move(Vector2f(0.f, direction2 * paddleSpeed * dt));
    
    float py2 = paddles[1].getPosition().y;
    if (py2 + paddleSize.y / 2 > gameHeight) {  // bottom boundary
        paddles[1].setPosition(paddles[1].getPosition().x, gameHeight - paddleSize.y / 2);
    }
    else if (py2 - paddleSize.y / 2 < 0) {  // top boundary
        paddles[1].setPosition(paddles[1].getPosition().x, paddleSize.y / 2);
    }

    // check ball collision
    const float bx = ball.getPosition().x;
    const float by = ball.getPosition().y;
    if (by > gameHeight) { //bottom wall
        // bottom wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, -10.f));
    }
    else if (by < 0) { //top wall
        // top wall
        ballVelocity.x *= velocityMultiplier;
        ballVelocity.y *= -velocityMultiplier;
        ball.move(Vector2f(0.f, 10.f));
    }

    else if (bx > gameWidth) {
        // right wall
        isPlayer1Serving = true;
        Reset();
    }
    else if (bx < 0) {
        // left wall
        isPlayer1Serving = false;
        Reset();
    }

    else if (
        //ball is inline or behind paddle AND
        bx < paddleSize.x + paddleOffsetWall &&
        //ball is below top edge of paddle AND
        by > paddles[0].getPosition().y - (paddleSize.y * 0.5) &&
        //ball is above bottom edge of paddle
        by < paddles[0].getPosition().y + (paddleSize.y * 0.5))
    {
        ballVelocity.x = -ballVelocity.x * velocityMultiplier;
    }
    else if (
        // ball is inline or behind the right paddle AND
        bx > gameWidth - paddleOffsetWall - paddleSize.x &&  // Adjust for right paddle's position
        // ball is below top edge of right paddle AND
        by > paddles[1].getPosition().y - (paddleSize.y * 0.5) &&
        // ball is above bottom edge of right paddle
        by < paddles[1].getPosition().y + (paddleSize.y * 0.5))
    {
        ballVelocity.x = -ballVelocity.x * velocityMultiplier;
    }
}

void Render(RenderWindow &window) {
    // Draw Everything
    window.draw(paddles[0]);
    window.draw(paddles[1]);
    window.draw(ball);
}

int main() {
    RenderWindow window(VideoMode(gameWidth, gameHeight), "PONG");
    Load();
    while (window.isOpen()) {
        window.clear();
        Update(window);
        Render(window);
        window.display();
    }
    return 0;
}