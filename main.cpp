#include <GL/glut.h>
#include <cmath>
#include <iostream>

// Các biến điều khiển camera
float cameraX = 0.0f;      // Vị trí X của camera (trái/phải)
float cameraY = 2.0f;      // Vị trí Y của camera (cao/thấp)
float cameraZ = 5.0f;      // Vị trí Z của camera (trước/sau)
float cameraAngle = 0.0f;  // Góc xoay camera theo trục Y (trái/phải)
float cameraPitch = 0.0f;  // Góc nghiêng camera lên/xuống
float cameraSpeed = 0.1f;  // Tốc độ di chuyển camera
float rotationSpeed = 0.05f; // Tốc độ xoay camera

// Các thông số ánh sáng chính (đèn trần)
GLfloat light_position[] = { 0.0f, 4.0f, 0.0f, 1.0f };  // Vị trí đèn chính ở giữa trần
GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // Ánh sáng môi trường yếu
GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // Ánh sáng khuếch tán mạnh

// Thông số ánh sáng cho đèn bàn (màu vàng ấm)
GLfloat lamp_light_ambient[] = { 0.2f, 0.2f, 0.1f, 1.0f };  // Ánh sáng môi trường màu vàng nhạt
GLfloat lamp_light_diffuse[] = { 1.0f, 0.9f, 0.5f, 1.0f };  // Ánh sáng khuếch tán màu vàng ấm

// Thông số ánh sáng cho đèn TV (màu trắng sáng khi bật)
GLfloat tv_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Không có ánh sáng môi trường khi tắt
GLfloat tv_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Ánh sáng trắng sáng khi bật
float tv_brightness = 0.0f;  // Độ sáng của TV (0.0 khi tắt, tăng dần khi bật)

// Thông số ánh sáng cho đèn tường
GLfloat wall_light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  // Ánh sáng môi trường yếu
GLfloat wall_light_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };  // Ánh sáng khuếch tán yếu

// Vị trí các đèn trong phòng
GLfloat lamp_light_position[] = { 0.6f, 0.9f, -0.4f, 1.0f };    // Đèn bàn trên góc bàn
GLfloat tv_light_position[] = { 0.0f, 1.8f, -4.9f, 1.0f };      // Đèn TV ở giữa màn hình
GLfloat wall_light1_position[] = { -4.8f, 2.5f, -2.0f, 1.0f };  // Đèn tường trái
GLfloat wall_light2_position[] = { 4.8f, 2.5f, -2.0f, 1.0f };   // Đèn tường phải

// Trạng thái các đèn (bật/tắt)
bool light0_enabled = true;   // Đèn chính (mặc định bật)
bool light1_enabled = false;  // Đèn bàn (mặc định tắt)
bool light2_enabled = false;  // Đèn TV (mặc định tắt)
bool light3_enabled = false;  // Đèn tường trái (mặc định tắt)
bool light4_enabled = false;  // Đèn tường phải (mặc định tắt)

// Hàm bật/tắt đèn
void toggleLight(int lightNum, bool& lightState) {
    lightState = !lightState;
    if (lightNum == 2) {  // Đèn TV (LIGHT2)
        if (lightState) {
            tv_brightness = 0.0f;  // Bắt đầu từ tối
            glEnable(GL_LIGHT2);
        } else {
            tv_brightness = 0.0f;  // Tắt ngay
            glDisable(GL_LIGHT2);
        }
    } else {
        if (lightState) {
            glEnable(GL_LIGHT0 + lightNum);
        } else {
            glDisable(GL_LIGHT0 + lightNum);
        }
    }
}

// Hàm bật/tắt tất cả đèn
void toggleAllLights() {
    bool newState = !light0_enabled;
    light0_enabled = newState;
    light1_enabled = newState;
    light2_enabled = newState;
    light3_enabled = newState;
    light4_enabled = newState;

    if (newState) {
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        tv_brightness = 0.0f;  // Bắt đầu từ tối
        glEnable(GL_LIGHT3);
        glEnable(GL_LIGHT4);
    } else {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        tv_brightness = 0.0f;
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
    }
}

// Hàm cập nhật độ sáng TV
void updateTVBrightness(int value) {
    if (light2_enabled && tv_brightness < 1.0f) {
        tv_brightness += 0.02f;  // Tăng độ sáng từ tối
        if (tv_brightness > 1.0f) tv_brightness = 1.0f;
        glutPostRedisplay();
        glutTimerFunc(16, updateTVBrightness, 0);  // 60 FPS
    }
}

// Hàm tính toán ma trận chiếu bằng
void computeShadowMatrix(GLfloat shadowMat[16], GLfloat lightPos[4], GLfloat plane[4]) {
    GLfloat dot = plane[0] * lightPos[0] + plane[1] * lightPos[1] + plane[2] * lightPos[2] + plane[3] * lightPos[3];

    shadowMat[0] = dot - lightPos[0] * plane[0];
    shadowMat[4] = 0.0f - lightPos[0] * plane[1];
    shadowMat[8] = 0.0f - lightPos[0] * plane[2];
    shadowMat[12] = 0.0f - lightPos[0] * plane[3];

    shadowMat[1] = 0.0f - lightPos[1] * plane[0];
    shadowMat[5] = dot - lightPos[1] * plane[1];
    shadowMat[9] = 0.0f - lightPos[1] * plane[2];
    shadowMat[13] = 0.0f - lightPos[1] * plane[3];

    shadowMat[2] = 0.0f - lightPos[2] * plane[0];
    shadowMat[6] = 0.0f - lightPos[2] * plane[1];
    shadowMat[10] = dot - lightPos[2] * plane[2];
    shadowMat[14] = 0.0f - lightPos[2] * plane[3];

    shadowMat[3] = 0.0f - lightPos[3] * plane[0];
    shadowMat[7] = 0.0f - lightPos[3] * plane[1];
    shadowMat[11] = 0.0f - lightPos[3] * plane[2];
    shadowMat[15] = dot - lightPos[3] * plane[3];
}

// Khai báo trước cho các hàm vẽ
void drawFloor();
void drawSofa();
void drawSofa1();
void drawTable();
void drawTV();
void drawVase();
void drawLamp();

// Hàm vẽ sàn nhà
void drawShadow() {
    GLfloat floorPlane[4] = { 0.0f, 1.0f, 0.0f, 0.0f };  // Mặt phẳng sàn (y = 0)
    GLfloat leftWallPlane[4] = { 1.0f, 0.0f, 0.0f, 5.0f };  // Mặt phẳng tường trái (x = -5)
    GLfloat shadowMat[16];

    glDisable(GL_LIGHTING);
    glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    if (light3_enabled && light4_enabled) {
        glEnable(GL_LIGHTING);
        glDisable(GL_BLEND);
        return;
    }

    glEnable(GL_STENCIL_TEST);

    // Vẽ bảng trên sàn
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    drawFloor();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Bảng từ LIGHT1 (đèn bàn) trên sàn
    if (light1_enabled) {
        computeShadowMatrix(shadowMat, lamp_light_position, floorPlane);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        drawSofa1();  // Vẽ bảng của sofa phụ
        glPopMatrix();
    }

    // Bảng từ LIGHT3 trên sàn
    if (light3_enabled) {
        computeShadowMatrix(shadowMat, wall_light1_position, floorPlane);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        drawSofa();
        drawSofa1();
        drawTable();
        drawTV();
        drawVase();
        drawLamp();
        glPopMatrix();
    }

    // Bảng từ LIGHT4 trên sàn
    if (light4_enabled) {
        computeShadowMatrix(shadowMat, wall_light2_position, floorPlane);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        drawSofa();
        drawSofa1();
        drawTable();
        drawTV();
        drawVase();
        drawLamp();
        glPopMatrix();
    }

    // Vẽ bảng trên tường bàn trái
    glStencilFunc(GL_ALWAYS, 2, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glBegin(GL_QUADS);  // Vẽ tường trái
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 2, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Bảng từ LIGHT1 (đèn bàn) trên tường trái
    if (light1_enabled) {
        computeShadowMatrix(shadowMat, lamp_light_position, leftWallPlane);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        drawSofa1();  // Vẽ bảng của sofa phụ
        glPopMatrix();
    }

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

// Hàm hiển thị hướng dẫn điều khiển
void displayControls() {
    std::cout << "\n=== HƯỚNG DẪN ĐIỀU KHIỂN ===\n\n";
    std::cout << "Điều khiển camera:\n";
    std::cout << "W: Di chuyển camera tiến lên\n";
    std::cout << "S: Di chuyển camera lùi xuống\n";
    std::cout << "A: Xoay camera sang trái\n";
    std::cout << "D: Xoay camera sang phải\n";
    std::cout << "Q: Di chuyển camera lên cao\n";
    std::cout << "E: Di chuyển camera xuống thấp\n";
    std::cout << "Z: Nghiêng camera lên\n";
    std::cout << "C: Nghiêng camera xuống\n";
    std::cout << "X: Tăng tốc độ di chuyển camera\n";
    std::cout << "V: Giảm tốc độ di chuyển camera\n\n";

    std::cout << "Điều khiển đèn:\n";
    std::cout << "1: Bật/tắt đèn chính (đèn trần)\n";
    std::cout << "2: Bật/tắt đèn bàn\n";
    std::cout << "3: Bật/tắt đèn TV\n";
    std::cout << "4: Bật/tắt đèn tường trái\n";
    std::cout << "5: Bật/tắt đèn tường phải\n";
    std::cout << "0: Bật/tắt tất cả đèn\n";
    std::cout << "\n==========================\n\n";
}

void init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Hiển thị hướng dẫn điều khiển
    displayControls();

    GLfloat global_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1f);
    if (!light0_enabled) glDisable(GL_LIGHT0);

    glLightfv(GL_LIGHT1, GL_POSITION, lamp_light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lamp_light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lamp_light_diffuse);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.4f);
    glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2f);
    if (!light1_enabled) glDisable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_POSITION, tv_light_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, tv_light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, tv_light_diffuse);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.3f);
    glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.1f);
    if (!light2_enabled) glDisable(GL_LIGHT2);

    glLightfv(GL_LIGHT3, GL_POSITION, wall_light1_position);
    glLightfv(GL_LIGHT3, GL_AMBIENT, wall_light_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, wall_light_diffuse);
    glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.3f);
    glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.1f);
    if (!light3_enabled) glDisable(GL_LIGHT3);

    glLightfv(GL_LIGHT4, GL_POSITION, wall_light2_position);
    glLightfv(GL_LIGHT4, GL_AMBIENT, wall_light_ambient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, wall_light_diffuse);
    glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.3f);
    glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.1f);
    if (!light4_enabled) glDisable(GL_LIGHT4);
}

// Hàm vẽ sàn nhà
void drawFloor() {
    glColor3f(0.8f, 0.8f, 0.8f);  // Màu xám nhạt
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glEnd();
}

// Hàm vẽ các bức tường
void drawWalls() {
    // Tường sau (gần TV)
    glColor3f(0.9f, 0.9f, 0.9f);  // Màu trắng nhạt
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(5.0f, 4.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glEnd();

    // Tường bên trái
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();

    // Tường bên phải
    glBegin(GL_QUADS);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glVertex3f(5.0f, 4.0f, -5.0f);
    glVertex3f(5.0f, 4.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glEnd();
}

// Hàm vẽ ghế sofa chính
void drawSofa() {
    glColor3f(0.4f, 0.2f, 0.1f);  // Màu nâu
    // Phần đệm ghế
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 3.0f);  // Ở giữa, gần tường trước
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(3.0f, 0.4f, 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Phần lưng ghế
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, 3.6f);
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(3.0f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Hàm vẽ ghế sofa phụ
void drawSofa1() {
    glColor3f(0.4f, 0.2f, 0.1f);  // Màu nâu
    // Phần đệm ghế phụ
    glPushMatrix();
    glTranslatef(-3.0f, 0.4f, 0.0f);  // Bên trái, vuông góc với sofa chính
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 0.4f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Phần lưng ghế phụ
    glPushMatrix();
    glTranslatef(-3.5f, 0.8f, 0.0f);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Hàm vẽ bàn
void drawTable() {
    // Mặt bàn
    glColor3f(0.6f, 0.4f, 0.2f);  // Màu nâu gỗ
    glPushMatrix();
    glTranslatef(0.0f, 0.7f, 0.0f);  // Ở giữa phòng
    glScalef(2.0f, 0.1f, 1.5f);  // Bàn rộng
    glutSolidCube(1.0f);
    glPopMatrix();

    // Chân bàn
    glColor3f(0.3f, 0.2f, 0.1f);
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glTranslatef(0.0f + i * 0.8f, 0.35f, 0.0f + j * 0.6f);
            glScalef(0.1f, 0.7f, 0.1f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }
}

// Hàm vẽ TV
void drawTV() {
    // Màn hình TV
    glColor3f(light2_enabled ? 1.0f * tv_brightness : 0.1f,
              light2_enabled ? 1.0f * tv_brightness : 0.1f,
              light2_enabled ? 1.0f * tv_brightness : 0.1f);  // Đen -> trắng
    glPushMatrix();
    glTranslatef(0.0f, 1.6f, -4.8f);  // Gần tường sau
    glScalef(2.0f, 1.2f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Chân đế TV
    glColor3f(0.2f, 0.2f, 0.2f);  // Màu xám đậm
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, -4.6f);
    glScalef(1.0f, 0.1f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Hàm vẽ rèm cửa
void drawCurtains() {
    // Thanh treo rèm
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.0f, 3.5f, -4.8f);  // Gần tường sau
    glScalef(4.0f, 0.05f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rèm
    glColor3f(0.8f, 0.8f, 1.0f);  // Màu xanh nhạt
    for (float x = -2.0f; x <= 2.0f; x += 0.2f) {
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, 3.5f, -4.85f);
        glVertex3f(x, 1.0f, -4.85f);
        glVertex3f(x + 0.1f, 1.0f, -4.85f);
        glVertex3f(x + 0.1f, 3.5f, -4.85f);
        glEnd();
    }
}

// Hàm vẽ tranh tường
void drawWallPainting() {
    // Khung tranh
    glColor3f(0.4f, 0.3f, 0.2f);
    glPushMatrix();
    glTranslatef(3.0f, 2.5f, -4.8f);  // Bên phải tường sau
    glScalef(0.8f, 0.6f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Nội dung tranh
    glColor3f(0.7f, 0.7f, 0.9f);  // Màu xanh nhạt
    glPushMatrix();
    glTranslatef(3.0f, 2.5f, -4.75f);
    glScalef(0.7f, 0.5f, 0.01f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

// Hàm vẽ lọ hoa
void drawVase() {
    // Thân lọ
    glColor3f(0.2f, 0.4f, 0.6f);  // Màu xanh dương
    glPushMatrix();
    glTranslatef(-0.4f, 0.9f, 0.3f);  // Đặt chậm bàn (y = 0.7 + 0.2)
    glScalef(0.2f, 0.4f, 0.2f);
    glutSolidSphere(1.0f, 16, 16);
    glPopMatrix();

    // Hoa
    glColor3f(0.8f, 0.2f, 0.2f);  // Màu đỏ
    for (int i = 0; i < 5; i++) {
        float angle = i * 72.0f * 3.14159f / 180.0f;
        glPushMatrix();
        glTranslatef(-0.4f + 0.1f * cos(angle), 1.1f, 0.3f + 0.1f * sin(angle));
        glScalef(0.05f, 0.2f, 0.05f);
        glutSolidSphere(1.0f, 8, 8);
        glPopMatrix();
    }
}

// Hàm vẽ thảm
void drawCarpet() {
    // Thảm
    glColor3f(0.6f, 0.3f, 0.3f);  // Màu đỏ nhạt
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-2.0f, 0.01f, -1.5f);
    glVertex3f(2.0f, 0.01f, -1.5f);
    glVertex3f(2.0f, 0.01f, 1.5f);
    glVertex3f(-2.0f, 0.01f, 1.5f);
    glEnd();

    // Họa tiết
    glColor3f(0.7f, 0.4f, 0.4f);
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.8f, 0.02f, -1.3f);
    glVertex3f(1.8f, 0.02f, -1.3f);
    glVertex3f(1.8f, 0.02f, 1.3f);
    glVertex3f(-1.8f, 0.02f, 1.3f);
    glEnd();
}

// Hàm vẽ đèn bàn
void drawLamp() {
    // Đế đèn
    glColor3f(0.3f, 0.3f, 0.3f);
    glPushMatrix();
    glTranslatef(0.6f, 0.75f, -0.4f);  // Đặt chậm bàn (y = 0.7 + 0.05)
    glScalef(0.2f, 0.1f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cột đèn
    glPushMatrix();
    glTranslatef(0.6f, 0.85f, -0.4f);
    glScalef(0.05f, 0.4f, 0.05f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Chụp đèn
    glColor3f(0.9f, 0.9f, 0.8f);
    glPushMatrix();
    glTranslatef(0.6f, 0.95f, -0.4f);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidCone(1.0f, 1.0f, 16, 16);
    glPopMatrix();
}

// Hàm vẽ đèn tường
void drawWallLights() {
    // Đèn tường trái
    glColor3f(0.8f, 0.8f, 0.8f);
    glPushMatrix();
    glTranslatef(-4.8f, 2.5f, -2.0f);
    glScalef(0.2f, 0.4f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Đèn tường phải
    glPushMatrix();
    glTranslatef(4.8f, 2.5f, -2.0f);
    glScalef(0.2f, 0.4f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ,
              cameraX + sin(cameraAngle) * cos(cameraPitch),
              cameraY + sin(cameraPitch),
              cameraZ - cos(cameraAngle) * cos(cameraPitch),
              0.0f, 1.0f, 0.0f);

    // Cập nhật độ sáng TV
    GLfloat tv_diffuse[] = { tv_brightness, tv_brightness, tv_brightness, 1.0f };
    glLightfv(GL_LIGHT2, GL_DIFFUSE, tv_diffuse);

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT1, GL_POSITION, lamp_light_position);
    glLightfv(GL_LIGHT2, GL_POSITION, tv_light_position);
    glLightfv(GL_LIGHT3, GL_POSITION, wall_light1_position);
    glLightfv(GL_LIGHT4, GL_POSITION, wall_light2_position);

    drawFloor();
    drawWalls();
    drawSofa();
    drawSofa1();
    drawTable();
    drawTV();
    drawCurtains();
    drawWallPainting();
    drawVase();
    drawCarpet();
    drawLamp();
    drawWallLights();

    drawShadow();

    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': cameraX += sin(cameraAngle) * cameraSpeed; cameraZ -= cos(cameraAngle) * cameraSpeed; break;
        case 's': cameraX -= sin(cameraAngle) * cameraSpeed; cameraZ += cos(cameraAngle) * cameraSpeed; break;
        case 'a': cameraAngle -= rotationSpeed; break;
        case 'd': cameraAngle += rotationSpeed; break;
        case 'q': cameraY += cameraSpeed; break;
        case 'e': cameraY -= cameraSpeed; break;
        case 'z': cameraPitch += rotationSpeed; break;
        case 'c': cameraPitch -= rotationSpeed; break;
        case 'x': cameraSpeed *= 1.1f; break;
        case 'v': cameraSpeed *= 0.9f; break;

        case '1': toggleLight(0, light0_enabled); break;
        case '2': toggleLight(1, light1_enabled); break;
        case '3': toggleLight(2, light2_enabled); if (light2_enabled) glutTimerFunc(0, updateTVBrightness, 0); break;
        case '4': toggleLight(3, light3_enabled); break;
        case '5': toggleLight(4, light4_enabled); break;
        case '0': toggleAllLights(); if (light2_enabled) glutTimerFunc(0, updateTVBrightness, 0); break;
    }
    glutPostRedisplay();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);
    glutInitWindowSize(800, 600);
    glutCreateWindow("3D Living Room");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}
