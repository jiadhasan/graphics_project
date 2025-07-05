#include <GL/glut.h>
#include <cmath>
#include <cstdlib> // For rand(), srand()
#include <vector>  // For std::vector
#include <ctime>   // For time() to seed srand()

// Global car positions
float redCarX2 = 100;
float blueCarX2 = 300;
float greenCarX2 = 550;
float carSpeed2 = 2.0f; // Initial speed
bool isCarMoving2 = true;

// Global cloud positions
float cloud1X2 = 150, cloud2X = 300, cloud3X = 500, cloud4X = 650;

// Plane position
float planeX2 = 200;

// Bus movement variables
float busX2 = 160;            // Initial position of the bus
bool isBusMoving2 = false;    // Whether the bus is currently moving
bool isReturning2 = false;    // Whether the bus is returning to stop

// Night mode flag
bool isNightMode2 = false;

// Rain variables
bool isRaining2 = false;
struct Raindrop {
    float x, y;
    float speed;
};
std::vector<Raindrop> raindrops;
const int NUM_RAINDROPS = 1000;    // rain ammount

// Thunder variables
bool isThundering2 = false;
int thunderFlashCount2 = 0; // Counts flashes within a thunder event
const int MAX_FLASH_COUNT2 = 3; // Number of thunder flashes
int thunderDelay2 = 0;      // Delay between flashes
const int MAX_THUNDER_DELAY2 = 10; // Frames between flashes
float lightningBrightness2 = 0.0f; // Controls the brightness of the flash

// Draw a circle
void drawCircle2(float centerX, float centerY, float radius, int segments = 100) {
    glBegin(GL_POLYGON);
    for (int i = 0; i < segments; i++) {
        float angle = 2.0f * 3.1416f * i / segments;
        float x = radius * cosf(angle);
        float y = radius * sinf(angle);
        glVertex2f(centerX + x, centerY + y);
    }
    glEnd();
}

// Draw the sun or moon based on night mode
void drawSunMoon2() {
    if (isNightMode2) {
        // Draw moon
        glColor3f(0.8f, 0.8f, 0.7f); // Pale yellow/white for moon
        drawCircle2(80, 510, 24);
        // Optional: draw some craters for moon effect
        glColor3f(0.6f, 0.6f, 0.5f);
        drawCircle2(75, 515, 5);
        drawCircle2(88, 505, 3);
    } else {
        // Draw sun
        glColor3f(1.0f, 1.0f, 0.0f); // Bright yellow for sun
        drawCircle2(80, 510, 24);
    }
}

// Draw a tree
void drawTree2(float x, float y) {
    // Trunk
    glColor3f(0.55f, 0.27f, 0.07f);
    glBegin(GL_QUADS);
        glVertex2f(x - 4, y);
        glVertex2f(x + 4, y);
        glVertex2f(x + 4, y + 60);
        glVertex2f(x - 4, y + 60);
    glEnd();

    // Leaves
    glColor3f(0.0f, 0.8f, 0.0f);
    glBegin(GL_TRIANGLES);
        glVertex2f(x - 24, y + 60);
        glVertex2f(x + 24, y + 60);
        glVertex2f(x, y + 120);
    glEnd();
}

// Draw a building with windows
void drawBuilding2(float x, float width, float height, float r, float g, float b) {
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, 180);
        glVertex2f(x + width, 180);
        glVertex2f(x + width, 180 + height);
        glVertex2f(x, 180 + height);
    glEnd();

    // Windows - color depends on night mode
    if (isNightMode2) {
        glColor3f(0.8f, 0.8f, 0.2f); // Yellowish light from windows at night
    } else {
        glColor3f(0.2f, 0.4f, 0.6f); // Dark bluish glass

    }

    for (float wx = x + 8; wx < x + width - 8; wx += 20) {
        for (float wy = 210; wy < 180 + height - 30; wy += 45) {
            glBegin(GL_QUADS);
                glVertex2f(wx, wy);
                glVertex2f(wx + 8, wy);
                glVertex2f(wx + 8, wy + 22.5);
                glVertex2f(wx, wy + 22.5);
            glEnd();
        }
    }
}

// Draw a simple car color
void drawCar2(float x, float y, float r, float g, float b) {
    // Car body
    glColor3f(r, g, b);
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 60, y);
        glVertex2f(x + 60, y + 20);
        glVertex2f(x, y + 20);
    glEnd();

    // Car top
    glColor3f(r * 0.8f, g * 0.8f, b * 0.8f);
    glBegin(GL_QUADS);
        glVertex2f(x + 10, y + 20);
        glVertex2f(x + 50, y + 20);
        glVertex2f(x + 45, y + 35);
        glVertex2f(x + 15, y + 35);
    glEnd();

    // Wheels
    glColor3f(0.0f, 0.0f, 0.0f);
    drawCircle2(x + 12, y - 2, 5);
    drawCircle2(x + 48, y - 2, 5);
}

// Draw the street
void drawStreet2() {
    glColor3f(0.3f, 0.3f, 0.3f);
    glBegin(GL_QUADS);
        glVertex2f(0, 30);
        glVertex2f(800, 30);
        glVertex2f(800, 150);
        glVertex2f(0, 150);
    glEnd();
}

// Draw zebra crossing
void drawZebraCrossing2(float x, float yStart, float totalHeight, int stripeCount, float stripeWidth) {
    float stripeHeight = totalHeight / (stripeCount * 2);  // Alternate gaps

    for (int i = 0; i < stripeCount; ++i) {
        float stripeY = yStart + i * 2 * stripeHeight;
        glColor3f(1.0f, 1.0f, 1.0f); // White stripes
        glBegin(GL_QUADS);
            glVertex2f(x, stripeY);
            glVertex2f(x + stripeWidth, stripeY);
            glVertex2f(x + stripeWidth, stripeY + stripeHeight);
            glVertex2f(x, stripeY + stripeHeight);
        glEnd();
    }
}

// Draw a footpath
void drawFootpath2(float yStart) {
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_QUADS);
        glVertex2f(0, yStart);
        glVertex2f(800, yStart);
        glVertex2f(800, yStart + 30);
        glVertex2f(0, yStart + 30);
    glEnd();
}

// Draw a lamp post
void drawLampPost2(float x, float y) {
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(x - 2.4f, y);
        glVertex2f(x + 2.4f, y);
        glVertex2f(x + 2.4f, y + 90);
        glVertex2f(x - 2.4f, y + 90);
    glEnd();

    if (isNightMode2) {
        glColor3f(1.0f, 1.0f, 0.6f); // Yellowish light at night
    } else {
        glColor3f(0.8f, 0.8f, 0.8f); // Off-white during day (no light)
    }
    drawCircle2(x, y + 95, 5.6f);
}

// draw waiting bench
void drawWaitingBench2(float x, float y) {
    glColor3f(0.4f, 0.2f, 0.0f);  // Brown bench

    // Seat
    glBegin(GL_QUADS);
        glVertex2f(x - 20, y);
        glVertex2f(x + 20, y);
        glVertex2f(x + 20, y + 6);
        glVertex2f(x - 20, y + 6);
    glEnd();

    // Backrest
    glBegin(GL_QUADS);
        glVertex2f(x - 20, y + 6);
        glVertex2f(x + 20, y + 6);
        glVertex2f(x + 20, y + 14);
        glVertex2f(x - 20, y + 14);
    glEnd();

    // Legs
    glBegin(GL_QUADS);
        glVertex2f(x - 18, y - 12);
        glVertex2f(x - 16, y - 12);
        glVertex2f(x - 16, y);
        glVertex2f(x - 18, y);

        glVertex2f(x + 16, y - 12);
        glVertex2f(x + 18, y - 12);
        glVertex2f(x + 18, y);
        glVertex2f(x + 16, y);
    glEnd();
}

// Draw a bench
void drawBench2(float x, float y) {
    glColor3f(0.5f, 0.25f, 0.0f);

    glBegin(GL_QUADS);
        glVertex2f(x - 16, y);
        glVertex2f(x + 16, y);
        glVertex2f(x + 16, y + 7.5);
        glVertex2f(x - 16, y + 7.5);
    glEnd();

    glBegin(GL_QUADS);
        glVertex2f(x - 14.4f, y - 22.5f);
        glVertex2f(x - 12.8f, y - 22.5f);
        glVertex2f(x - 12.8f, y);
        glVertex2f(x - 14.4f, y);

        glVertex2f(x + 12.8f, y - 22.5f);
        glVertex2f(x + 14.4f, y - 22.5f);
        glVertex2f(x + 14.4f, y);
        glVertex2f(x + 12.8f, y);
    glEnd();
}

// Fixed background buildings (skyline)

void drawBackgroundSkyline2() {
    float x = 0;
    const float buildingWidths[] = {60, 80, 100, 70, 50, 90, 60, 85, 65, 85};
    const float buildingHeights[] = {160, 200, 160, 140, 170, 190, 150, 175, 160, 185};
    int count = sizeof(buildingWidths) / sizeof(buildingWidths[0]);

    for (int i = 0; i < count; ++i) {
        glColor3f(0.4f, 0.4f, 0.5f);  // Gray-blue tint for background
        glBegin(GL_QUADS);
            glVertex2f(x, 180);
            glVertex2f(x + buildingWidths[i], 180);
            glVertex2f(x + buildingWidths[i], 180 + buildingHeights[i]);
            glVertex2f(x, 180 + buildingHeights[i]);
        glEnd();
        x += buildingWidths[i] + 10;  // gap between buildings
    }
}

void drawCloud2(float x, float y) {
    glColor3f(1.0f, 1.0f, 1.0f); // White color for cloud

    // Draw 5 overlapping circles to make a cloud
    drawCircle2(x, y, 15);
    drawCircle2(x + 15, y + 5, 15);
    drawCircle2(x + 30, y, 15);
    drawCircle2(x + 10, y - 10, 15);
    drawCircle2(x + 20, y - 10, 15);
}

void drawAirplane2(float x, float y) {
    // Body of the airplane

    glColor3f(0.675f, 0.675f, 0.675f); // Medium-light ash




    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 60, y);
        glVertex2f(x + 60, y + 10);
        glVertex2f(x, y + 10);
    glEnd();

    // Nose (triangle at front)
    glBegin(GL_TRIANGLES);
        glVertex2f(x + 60, y);
        glVertex2f(x + 60, y + 10);
        glVertex2f(x + 70, y + 5);
    glEnd();

    // Tail wing
    glBegin(GL_TRIANGLES);
        glVertex2f(x, y + 10);
        glVertex2f(x - 10, y + 15);
        glVertex2f(x, y);
    glEnd();

    // Main wing (top)
    glBegin(GL_TRIANGLES);
        glVertex2f(x + 20, y + 10);
        glVertex2f(x + 40, y + 10);
        glVertex2f(x + 30, y + 25);
    glEnd();
}


//bus color
void drawBus2(float x, float y) {
    // Bus body
    glColor3f(1.0f, 0.7f, 0.0f);  // Orange-yellow
    glBegin(GL_QUADS);
        glVertex2f(x, y);
        glVertex2f(x + 120, y);
        glVertex2f(x + 120, y + 30);
        glVertex2f(x, y + 30);
    glEnd();

    // Windows
    glColor3f(0.2f, 0.6f, 1.0f); // Blue windows
    for (int i = 0; i < 4; ++i) {
        float winX = x + 10 + i * 25;
        glBegin(GL_QUADS);
            glVertex2f(winX, y + 10);
            glVertex2f(winX + 20, y + 10);
            glVertex2f(winX + 20, y + 25);
            glVertex2f(winX, y + 25);
        glEnd();
    }

    // Wheels
    glColor3f(0.0f, 0.0f, 0.0f); // Black wheels
    drawCircle2(x + 20, y - 4, 6);
    drawCircle2(x + 100, y - 4, 6);
}

void drawBus2StopSign2(float x, float y) {
    // Pole
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
        glVertex2f(x - 2, y);
        glVertex2f(x + 2, y);
        glVertex2f(x + 2, y + 40);
        glVertex2f(x - 2, y + 40);
    glEnd();

    // Signboard
    glColor3f(1.0f, 1.0f, 0.0f); // Yellow background
    glBegin(GL_QUADS);
        glVertex2f(x - 20, y + 40);
        glVertex2f(x + 20, y + 40);
        glVertex2f(x + 20, y + 60);
        glVertex2f(x - 20, y + 60);
    glEnd();

    // "BUS STOP" text (symbolic using 3 lines)
    glColor3f(1.0f, 0.0f, 0.0f);
    glLineWidth(2.0);
    glBegin(GL_LINES);
        glVertex2f(x - 15, y + 50);
        glVertex2f(x + 15, y + 50);
    glEnd();
}

// rain size

void drawRain2() {
    if (!isRaining2) return;

    glColor4f(1.0f, 1.0f, 1.0f, 0.6f);//glColor4f(1.0f, 0.0f, 0.0f, 0.6f); // Light blue/gray, semi-transparent
    glBegin(GL_LINES);
    for (const auto& drop : raindrops) {
        glVertex2f(drop.x, drop.y);
        glVertex2f(drop.x, drop.y + 5); // Draw as short lines
    }
    glEnd();
}

// update functions

void update2(int value) {
    // Move plane to the right
    planeX2 += 1.5f;
    if (planeX2 > 800) planeX2 = -70;

    // Move clouds to the left
    cloud1X2 -= 0.5f;
    cloud2X -= 0.13f;
    cloud3X -= 0.13f;
    cloud4X -= 0.13f;

    if (cloud1X2 < -50) cloud1X2 = 850;
    if (cloud2X < -50) cloud2X = 850;
    if (cloud3X < -50) cloud3X = 850;
    if (cloud4X < -50) cloud4X = 850;

    // Move cars only if enabled
    if (isCarMoving2) {
        redCarX2 -= carSpeed2;// lower lane lane
        greenCarX2 -= carSpeed2;//lower lane
        blueCarX2 += carSpeed2;//upper lane

        if (redCarX2 < -60) redCarX2 = 800;
        if (greenCarX2 < -90) greenCarX2 = 800;
        if (blueCarX2 > 800) blueCarX2 = -60;
    }

    // Bus movement bus speed

    if (isBusMoving2) {
    busX2 += 1.0f; // Always move right

    // When it fully disappears on the right, teleport it to left side
    if (busX2 > 800 && !isReturning2) {
        busX2 = -120;         // Move to left off-screen
        isReturning2 = true;  // Now it's returning to stop
    }

    // When bus reaches the stop again from left
    if (isReturning2 && busX2 >= 160) {
        busX2 = 160;
        isBusMoving2 = false;
        isReturning2 = false;
    }
}

 /* if (isBusMoving2) {
    busX2 -= 1.0f; // Always move right

    // When it fully disappears on the right, teleport it to left side
    if (busX2 < -90 && !isReturning2) {
        busX2 = 820;         // Move to left off-screen
        isReturning2 = true;  // Now it's returning to stop
    }

    // When bus reaches the stop again from left
    if (isReturning2 && busX2 <= 160) {
        busX2 = 160;
        isBusMoving2 = false;
        isReturning2 = false;
    }
}
*/



    // Rain update2
    if (isRaining2) {
        for (auto& drop : raindrops) {
            drop.y -= drop.speed;
            if (drop.y < 0) {
                drop.y = 600 + (rand() % 20); // Reset above screen with slight variation
                drop.x = rand() % 800;       // Random X
                drop.speed = 5.0f + (rand() % 100 / 1.0f); // Slight speed variation


            }

        }
    }

    // Thunder update2
    if (isThundering2) {
        thunderDelay2++;
        if (thunderDelay2 >= MAX_THUNDER_DELAY2) {
            if (thunderFlashCount2 < MAX_FLASH_COUNT2 * 2) { // Two states per flash (on/off)
                if (thunderFlashCount2 % 2 == 0) { // Flash on
                    lightningBrightness2 = 1.0f;
                } else { // Flash off
                    lightningBrightness2 = 0.0f;
                }
                thunderFlashCount2++;
            } else {
                isThundering2 = false;
                lightningBrightness2 = 0.0f;
                thunderFlashCount2 = 0;
                thunderDelay2 = 0;
            }
            thunderDelay2 = 0; // Reset delay for next flash or end
        }
    }


    glutPostRedisplay(); // Request redraw
    glutTimerFunc(16, update2, 0); // Call again after 16ms (~60 FPS)
}

void drawBigStars2() {
    if (!isNightMode2) return;

    glColor3f(1.0f, 1.0f, 1.0f); // Bright white color for stars

    // Manually place 4–5 stars at fixed positions
    drawCircle2(100, 550, 4);
    drawCircle2(200, 520, 5);
    drawCircle2(400, 560, 6);
    drawCircle2(600, 540, 4.5);
    drawCircle2(700, 580, 5.5);
}





// Display function


void display2() {
    // Base clear color based on night mode
    float clearR, clearG, clearB;
    if (isNightMode2) {
        clearR = 0.05f; clearG = 0.05f; clearB = 0.2f; // Dark blue for night sky
    } else {
        clearR = 0.53f; clearG = 0.81f; clearB = 0.98f; // Light blue for day sky
    }

    // Adjust clear color for thunder flash
    if (isThundering2) {
        clearR = 0.8f * lightningBrightness2 + clearR * (1.0f - lightningBrightness2);
        clearG = 0.8f * lightningBrightness2 + clearG * (1.0f - lightningBrightness2);
        clearB = 0.9f * lightningBrightness2 + clearB * (1.0f - lightningBrightness2);
    }
    glClearColor(clearR, clearG, clearB, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Enable blending for transparency (for rain)
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    drawSunMoon2();

    drawBigStars2();

    // Draw clouds (hide or make them darker at night)
    if (!isNightMode2) {
        drawCloud2(cloud1X2, 520);
        drawCloud2(cloud2X, 540);
        drawCloud2(cloud3X, 500);
        drawCloud2(cloud4X, 530);
    } // Clouds are omitted in night mode or during thunder for effect

    // Position of the airplane
    drawAirplane2(planeX2, 560);

    drawBackgroundSkyline2(); // Add this here for background

    // Front buildings
    drawBuilding2(16, 64, 240, 0.6f, 0.4f, 0.7f);       // fbuilding 1
    drawBuilding2(96, 80, 225, 0.3f, 0.7f, 0.9f);      // fbuilding 2
    drawBuilding2(180, 64, 150, 0.2f, 0.1f, 0.0f);      // fbuilding 3
    drawBuilding2(256, 80, 270, 0.8f, 0.6f, 0.6f);       //fb6
    drawBuilding2(416, 96, 240, 0.6f, 0.8f, 0.6f);
    drawBuilding2(576, 80, 210, 0.9f, 0.9f, 0.5f);
    drawBuilding2(696, 32, 180, 0.5f, 0.5f, 0.5f);
    drawBuilding2(736, 64, 255, 1.0f, 0.0f, 0.0f);




    drawTree2(120, 180);
    drawTree2(280, 180);
    drawTree2(440, 180);
    drawTree2(600, 180);
    drawTree2(760, 180);


    drawStreet2();
    drawFootpath2(0);
    drawFootpath2(150);

    drawZebraCrossing2(380, 40, 100, 8, 40);

    // Bus Stopage
    drawBus2StopSign2(170, 150);     // Signboard near stop
    drawWaitingBench2(220, 170);    // Bench inside shelter

    // Cars and Bus

    drawBus2(busX2, 115);

    drawCar2(blueCarX2, 90, 0.0f, 0.0f, 1.0f);   // Blue car - right
    drawCar2(redCarX2, 60, 1.0f, 0.0f, 0.0f);    // Red car - left
     drawCar2(greenCarX2, 60, 0.0f, 1.0f, 0.0f);// Green car - left

    // Lamposts and benches
    drawBench2(160, 7.5);
    drawLampPost2(192, 30);

    drawBench2(400, 7.5);
    drawLampPost2(432, 30);

    drawBench2(640, 7.5);
    drawLampPost2(672, 30);

    drawRain2(); // Draw rain after all other elements

    glDisable(GL_BLEND); // Disable blending

    glFlush();
}

// Initialization
void init2() {
    srand(time(0)); // Seed random number generator

    // Initialize raindrops
    for (int i = 0; i < NUM_RAINDROPS; ++i) {
        raindrops.push_back({
            (float)(rand() % 800),         // x
            (float)(rand() % 600 + 600),   // y (start above screen)
            2.0f + (rand() % 100 / 100.0f) // speed (2.0 to 2.99)
        });
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600); // New coordinate system
}

// Keyboard handler for car, night mode, and day mode
void handleKeyboard2(unsigned char key, int x, int y) {
    if (key == 'S' || key == 's') { // Toggle car movement
        isCarMoving2 = !isCarMoving2;

    }

     else if (key == 'C' || key == 'c') { // Increase car speed
        carSpeed2 += 0.5f;

    }

         else if (key == 'E' || key == 'e') { // Increase car speed
        carSpeed2 -= 0.5f;

    }

    else if (key == 'B' || key == 'b') { // Start bus movement
        if (!isBusMoving2) {
            isBusMoving2 = true;
            isReturning2 = false; // Ensure bus starts moving forward initially
        }

    } else if (key == 'N' || key == 'n') { // Toggle night mode
        isNightMode2 = !isNightMode2;
    } else if (key == 'D' || key == 'd') { // Set to day mode
        isNightMode2 = false;
    }
    glutPostRedisplay(); // Request redraw after key press
}

// Mouse click handler for rain and thunder
void handleMouse2(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == GLUT_LEFT_BUTTON) {
            isRaining2 = !isRaining2; // Toggle rain
        } else if (button == GLUT_RIGHT_BUTTON) {
            if (!isThundering2) { // Only start if not already thundering
                isThundering2 = true;
                thunderFlashCount2 = 0;
                thunderDelay2 = 0;
                lightningBrightness2 = 0.0f; // Start dark, then flash
            }
        }
    }
    glutPostRedisplay(); // Request redraw
}


// Main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600); // Window size
    glutCreateWindow("Scaled City Scene");
    init2();
    glutDisplayFunc(display2);
    glutTimerFunc(0, update2, 0); // Start animation timer
    glutKeyboardFunc(handleKeyboard2);
    glutMouseFunc(handleMouse2); // Register mouse function

    glutMainLoop();
    return 0;
}
