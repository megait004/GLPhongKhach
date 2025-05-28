#include <GL/glut.h>
#include <cmath>
#include <string>

// ===== CÁC BIẾN ĐIỀU KHIỂN CAMERA =====
float cameraX = 0.0f;      // Vị trí X của camera
float cameraY = 2.0f;      // Vị trí Y của camera (chiều cao)
float cameraZ = 5.0f;      // Vị trí Z của camera
float cameraAngle = 0.0f;  // Góc xoay ngang của camera
float cameraPitch = 0.0f;  // Góc xoay dọc của camera
float cameraSpeed = 0.1f;  // Tốc độ di chuyển camera
float rotationSpeed = 0.05f; // Tốc độ xoay camera

// ===== THAM SỐ ÁNH SÁNG =====
// Đèn trần trung tâm
GLfloat light0_position[] = { 0.0f, 3.9f, 0.0f, 1.0f };  // Vị trí đèn trần trung tâm
GLfloat light0_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };   // Ánh sáng môi trường
GLfloat light0_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };   // Ánh sáng khuếch tán

// Đèn trần góc
GLfloat light1_position[] = { -4.5f, 3.9f, -4.5f, 1.0f };  // Góc trên trái
GLfloat light2_position[] = { 4.5f, 3.9f, -4.5f, 1.0f };   // Góc trên phải
GLfloat light3_position[] = { -4.5f, 3.9f, 4.5f, 1.0f };   // Góc dưới trái
GLfloat light4_position[] = { 4.5f, 3.9f, 4.5f, 1.0f };    // Góc dưới phải
GLfloat corner_light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f }; // Ánh sáng môi trường yếu
GLfloat corner_light_diffuse[] = { 0.4f, 0.4f, 0.4f, 1.0f }; // Ánh sáng khuếch tán yếu

// Đèn bàn (vàng)
GLfloat lamp_light_ambient[] = { 0.2f, 0.2f, 0.1f, 1.0f };  // Ánh sáng môi trường vàng
GLfloat lamp_light_diffuse[] = { 1.0f, 0.9f, 0.5f, 1.0f };  // Ánh sáng khuếch tán vàng
GLfloat lamp_light_position[] = { 0.6f, 0.9f, -0.4f, 1.0f }; // Vị trí đèn bàn

// Đèn TV (trắng khi bật)
GLfloat tv_light_ambient[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Không có ánh sáng môi trường khi tắt
GLfloat tv_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };  // Ánh sáng khuếch tán trắng khi bật
GLfloat tv_light_position[] = { 0.0f, 1.8f, -4.9f, 1.0f }; // Vị trí đèn TV
float tv_brightness = 0.0f;  // Độ sáng TV (0.0 khi tắt)

// Đèn tường
GLfloat wall_light_ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };  // Ánh sáng môi trường yếu
GLfloat wall_light_diffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };  // Ánh sáng khuếch tán yếu
GLfloat wall_light_position[] = { -4.8f, 2.5f, -2.0f, 1.0f };  // Đèn tường trái

// ===== TRẠNG THÁI ĐÈN =====
bool light0_enabled = true;   // Đèn trần trung tâm (mặc định bật)
bool light1_enabled = true;   // Đèn góc trên trái (mặc định bật)
bool light2_enabled = true;   // Đèn góc trên phải (mặc định bật)
bool light3_enabled = true;   // Đèn góc dưới trái (mặc định bật)
bool light4_enabled = true;   // Đèn góc dưới phải (mặc định bật)
bool light5_enabled = false;  // Đèn bàn (mặc định tắt)
bool light6_enabled = false;  // Đèn TV (mặc định tắt)
bool light7_enabled = false;  // Đèn tường trái (mặc định tắt)

// ===== CÁC HÀM ĐIỀU KHIỂN ĐÈN =====

// Hàm bật/tắt đèn
void toggleLight(int lightNum, bool& lightState) {
    lightState = !lightState;  // Đảo trạng thái đèn
    if (lightNum == 6) {  // Đèn TV (LIGHT6)
        if (lightState) {
            tv_brightness = 0.0f;  // Bắt đầu tối
            glEnable(GL_LIGHT6);
        } else {
            tv_brightness = 0.0f;  // Tắt
            glDisable(GL_LIGHT6);
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
    bool newState = !light0_enabled;  // Lấy trạng thái ngược lại của đèn trần trung tâm
    light0_enabled = newState;
    light1_enabled = newState;
    light2_enabled = newState;
    light3_enabled = newState;
    light4_enabled = newState;
    light5_enabled = newState;
    light6_enabled = newState;
    light7_enabled = newState;

    if (newState) {
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
        glEnable(GL_LIGHT4);
        glEnable(GL_LIGHT5);
        glEnable(GL_LIGHT6);
        tv_brightness = 0.0f;  // Bắt đầu tối
        glEnable(GL_LIGHT7);
    } else {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
        glDisable(GL_LIGHT5);
        glDisable(GL_LIGHT6);
        tv_brightness = 0.0f;
        glDisable(GL_LIGHT7);
    }
}

// Hàm cập nhật độ sáng TV
void updateTVBrightness(int value) {
    if (light6_enabled && tv_brightness < 1.0f) {
        tv_brightness += 0.02f;  // Tăng dần độ sáng
        if (tv_brightness > 1.0f) tv_brightness = 1.0f;
        glutPostRedisplay();
        glutTimerFunc(16, updateTVBrightness, 0);  // 60 FPS
    }
}

// Ham tinh ma tran bong
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

// Tuyen bo cac ham ve
void drawFloor();
void drawCeiling();
void drawCeilingLights();
void drawSofa();
void drawSofa1();
void drawTable();
void drawTV();
void drawVase();
void drawLamp();
void drawWallPainting();
void drawCurtains();
void drawCarpet();
void drawWallLights();

// Ham ve bong
void drawShadow() {
    GLfloat floorPlane[4] = { 0.0f, 1.0f, 0.0f, 0.0f };  // Mat phang san (y = 0)
    GLfloat leftWallPlane[4] = { 1.0f, 0.0f, 0.0f, 5.0f };  // Mat phang tuong trai (x = -5)
    GLfloat shadowMat[16];

    glDisable(GL_LIGHTING);
    glColor4f(0.1f, 0.1f, 0.1f, 0.5f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_STENCIL_TEST);

    // Bong tren san
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    drawFloor();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Bong tu den ban (LIGHT5) tren san
    if (light5_enabled) {
        computeShadowMatrix(shadowMat, lamp_light_position, floorPlane);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        drawSofa();
        drawSofa1();
        drawTable();
        drawVase();
        drawLamp();
        glPopMatrix();
    }

    // Bong tu den tuong (LIGHT7) tren san
    if (light7_enabled) {
        computeShadowMatrix(shadowMat, wall_light_position, floorPlane);
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

    // Bong tren tuong trai
    glStencilFunc(GL_ALWAYS, 2, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glDepthMask(GL_FALSE);
    glBegin(GL_QUADS);  // Tuong trai
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glDepthMask(GL_TRUE);
    glStencilFunc(GL_EQUAL, 2, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    // Bong tu den ban (LIGHT5) tren tuong trai
    if (light5_enabled) {
        computeShadowMatrix(shadowMat, lamp_light_position, leftWallPlane);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        drawSofa();
        drawSofa1();
        drawTable();
        drawVase();
        drawLamp();
        glPopMatrix();
    }

    // Bong tu den tuong (LIGHT7) tren tuong trai
    if (light7_enabled) {
        computeShadowMatrix(shadowMat, wall_light_position, leftWallPlane);
        glPushMatrix();
        glMultMatrixf(shadowMat);
        drawSofa();
        drawSofa1();
        drawTable();
        drawVase();
        drawLamp();
        glPopMatrix();
    }

    glDisable(GL_STENCIL_TEST);
    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);
}

// Hàm khởi tạo OpenGL
void init() {
    glEnable(GL_DEPTH_TEST);  // Bật kiểm tra độ sâu
    glEnable(GL_STENCIL_TEST);  // Bật kiểm tra stencil
    glEnable(GL_LIGHTING);  // Bật ánh sáng
    glEnable(GL_COLOR_MATERIAL);  // Bật vật liệu màu
    glEnable(GL_BLEND);  // Bật blend
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Cài đặt hàm blend

    // Ánh sáng môi trường toàn cục
    GLfloat global_ambient[] = { 0.05f, 0.05f, 0.05f, 1.0f };
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);

    // Đèn trần trung tâm (LIGHT0)
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5f);  // Suy hao hằng số
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1f);    // Suy hao tuyến tính
    if (!light0_enabled) glDisable(GL_LIGHT0);

    // Đèn trần góc (LIGHT1 đến LIGHT4)
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, corner_light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, corner_light_diffuse);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2f);
    if (!light1_enabled) glDisable(GL_LIGHT1);

    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, corner_light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, corner_light_diffuse);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.2f);
    if (!light2_enabled) glDisable(GL_LIGHT2);

    glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
    glLightfv(GL_LIGHT3, GL_AMBIENT, corner_light_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, corner_light_diffuse);
    glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.2f);
    if (!light3_enabled) glDisable(GL_LIGHT3);

    glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
    glLightfv(GL_LIGHT4, GL_AMBIENT, corner_light_ambient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, corner_light_diffuse);
    glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.2f);
    if (!light4_enabled) glDisable(GL_LIGHT4);

    // Đèn bàn (LIGHT5)
    glLightfv(GL_LIGHT5, GL_POSITION, lamp_light_position);
    glLightfv(GL_LIGHT5, GL_AMBIENT, lamp_light_ambient);
    glLightfv(GL_LIGHT5, GL_DIFFUSE, lamp_light_diffuse);
    glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.6f);
    glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.3f);  // Suy hao bậc hai
    if (!light5_enabled) glDisable(GL_LIGHT5);

    // Đèn TV (LIGHT6)
    glLightfv(GL_LIGHT6, GL_POSITION, tv_light_position);
    glLightfv(GL_LIGHT6, GL_AMBIENT, tv_light_ambient);
    glLightfv(GL_LIGHT6, GL_DIFFUSE, tv_light_diffuse);
    glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.3f);
    glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.1f);
    if (!light6_enabled) glDisable(GL_LIGHT6);

    // Đèn tường (LIGHT7)
    glLightfv(GL_LIGHT7, GL_POSITION, wall_light_position);
    glLightfv(GL_LIGHT7, GL_AMBIENT, wall_light_ambient);
    glLightfv(GL_LIGHT7, GL_DIFFUSE, wall_light_diffuse);
    glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.3f);
    glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0.1f);
    if (!light7_enabled) glDisable(GL_LIGHT7);
}

// ===== CÁC HÀM VẼ ĐỐI TƯỢNG =====

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

// Hàm vẽ trần nhà
void drawCeiling() {
    glColor3f(0.9f, 0.9f, 0.9f);  // Màu trắng nhạt
    glBegin(GL_QUADS);
    glNormal3f(0.0f, -1.0f, 0.0f);  // Vector pháp tuyến hướng xuống
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, 5.0f);
    glVertex3f(5.0f, 4.0f, 5.0f);
    glVertex3f(5.0f, 4.0f, -5.0f);
    glEnd();
}

// Hàm vẽ đèn trần
void drawCeilingLights() {
    // Đèn trần vuông trung tâm (LIGHT0)
    glPushMatrix();
    glTranslatef(0.0f, 3.9f, 0.0f);  // Trung tâm trần
    glScalef(1.0f, 0.05f, 1.0f);  // Hình vuông
    if (light0_enabled) {
        GLfloat emission[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glColor3f(1.0f, 1.0f, 1.0f);  // Màu trắng sáng
    } else {
        GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        GLfloat diffuse[] = { 0.3f, 0.3f, 0.3f, 1.0f };
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glColor3f(0.3f, 0.3f, 0.3f);  // Màu xám tối
    }
    glutSolidCube(1.0f);
    glPopMatrix();

    // Đèn góc (LIGHT1 đến LIGHT4)
    GLfloat corner_positions[4][3] = {
        {-4.5f, 3.9f, -4.5f},  // Góc trên trái
        {4.5f, 3.9f, -4.5f},   // Góc trên phải
        {-4.5f, 3.9f, 4.5f},   // Góc dưới trái
        {4.5f, 3.9f, 4.5f}     // Góc dưới phải
    };
    bool corner_enabled[4] = {light1_enabled, light2_enabled, light3_enabled, light4_enabled};

    for (int i = 0; i < 4; i++) {
        glPushMatrix();
        glTranslatef(corner_positions[i][0], corner_positions[i][1], corner_positions[i][2]);
        glScalef(0.5f, 0.05f, 0.5f);  // Vuông nhỏ hơn cho các góc
        if (corner_enabled[i]) {
            GLfloat emission[] = { 0.4f, 0.4f, 0.4f, 1.0f };
            glMaterialfv(GL_FRONT, GL_EMISSION, emission);
            glColor3f(0.8f, 0.8f, 0.8f);  // Màu trắng mềm
        } else {
            GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
            glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
            GLfloat diffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };
            glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
            glColor3f(0.2f, 0.2f, 0.2f);  // Màu xám tối
        }
        glutSolidCube(1.0f);
        glPopMatrix();
    }

    // Đặt lại thuộc tính vật liệu
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
}

// Hàm vẽ tường
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

    // Tường trái
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();

    // Tường phải
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
    // Thân ghế sofa
    glPushMatrix();
    glTranslatef(0.0f, 0.4f, 3.0f);  // Trung tâm, gần tường trước
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(3.0f, 0.4f, 1.2f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Lưng ghế sofa
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
    // Thân ghế sofa phụ
    glPushMatrix();
    glTranslatef(-3.0f, 0.4f, 0.0f);  // Bên trái, vuông góc với sofa chính
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 0.4f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Lưng ghế sofa phụ
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
    glTranslatef(0.0f, 0.7f, 0.0f);  // Trung tâm phòng
    glScalef(2.0f, 0.1f, 1.5f);  // Bàn rộng
    glutSolidCube(1.0f);
    glPopMatrix();

    // Chân bàn với độ phản xạ thấp để giữ tối
    glDisable(GL_COLOR_MATERIAL);  // Tắt GL_COLOR_MATERIAL để kiểm soát vật liệu
    glColor3f(0.3f, 0.2f, 0.1f);  // Màu nâu tối
    GLfloat leg_ambient[] = { 0.1f, 0.05f, 0.05f, 1.0f };  // Ánh sáng môi trường thấp
    GLfloat leg_diffuse[] = { 0.3f, 0.2f, 0.1f, 1.0f };  // Phản xạ khuếch tán thấp
    GLfloat leg_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };  // Không phản xạ gương
    glMaterialfv(GL_FRONT, GL_AMBIENT, leg_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, leg_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, leg_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);  // Không độ bóng
    for (int i = -1; i <= 1; i += 2) {
        for (int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glTranslatef(0.0f + i * 0.8f, 0.35f, 0.0f + j * 0.6f);
            glScalef(0.1f, 0.7f, 0.1f);
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }

    // Bật lại GL_COLOR_MATERIAL và đặt lại thuộc tính vật liệu mặc định
    glEnable(GL_COLOR_MATERIAL);
    GLfloat default_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_AMBIENT, default_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
}

// Hàm vẽ TV
void drawTV() {
    // Màn hình TV
    glPushMatrix();
    glTranslatef(0.0f, 1.6f, -4.8f);  // Gần tường sau
    glScalef(2.0f, 1.2f, 0.2f);
    glColor3f(light6_enabled ? 1.0f * tv_brightness : 0.1f,
              light6_enabled ? 1.0f * tv_brightness : 0.1f,
              light6_enabled ? 1.0f * tv_brightness : 0.1f);  // Đèn trắng khi bật
    GLfloat tv_diffuse[] = { light6_enabled ? tv_brightness : 0.1f, light6_enabled ? tv_brightness : 0.1f, light6_enabled ? tv_brightness : 0.1f, 1.0f };
    GLfloat tv_specular[] = { 0.3f, 0.3f, 0.3f, 1.0f };  // Phản xạ kính nhẹ
    glMaterialfv(GL_FRONT, GL_DIFFUSE, tv_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, tv_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0f);  // Độ bóng vừa phải
    glutSolidCube(1.0f);
    glPopMatrix();

    // Giá đỡ TV
    glColor3f(0.2f, 0.2f, 0.2f);  // Xám tối
    glPushMatrix();
    glTranslatef(0.0f, 0.8f, -4.6f);
    glScalef(1.0f, 0.1f, 0.4f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Đặt lại thuộc tính vật liệu mặc định
    GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
    GLfloat default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
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
    glColor3f(0.8f, 0.8f, 1.0f);  // Xanh nhạt
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
    glPushMatrix();
    glTranslatef(3.0f, 2.5f, -4.8f);  // Bên phải tường sau
    glScalef(0.8f, 0.6f, 0.05f);
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    GLfloat frame_diffuse[] = { 0.4f, 0.3f, 0.2f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, frame_diffuse);
    glColor3f(0.4f, 0.3f, 0.2f);  // Khung nâu
    glutSolidCube(1.0f);
    glPopMatrix();

    // Nội dung tranh
    glPushMatrix();
    glTranslatef(3.0f, 2.5f, -4.75f);
    glScalef(0.7f, 0.5f, 0.01f);
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);  // Không phát sáng
    GLfloat painting_diffuse[] = { 0.3f, 0.3f, 0.4f, 1.0f };  // Xanh tối
    glMaterialfv(GL_FRONT, GL_DIFFUSE, painting_diffuse);
    GLfloat painting_specular[] = { 0.05f, 0.05f, 0.05f, 1.0f };  // Phản xạ kính rất thấp
    glMaterialfv(GL_FRONT, GL_SPECULAR, painting_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 5.0f);  // Độ bóng rất thấp
    glColor3f(0.3f, 0.3f, 0.4f);  // Xanh tối
    glutSolidCube(1.0f);
    glPopMatrix();

    // Đặt lại thuộc tính vật liệu mặc định
    GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
    GLfloat default_specular[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_SPECULAR, default_specular);
    glMaterialf(GL_FRONT, GL_SHININESS, 0.0f);
}

// Hàm vẽ bình hoa
void drawVase() {
    // Thân bình hoa
    glColor3f(0.2f, 0.4f, 0.6f);  // Xanh
    glPushMatrix();
    glTranslatef(-0.4f, 0.9f, 0.3f);  // Trên bàn
    glScalef(0.2f, 0.4f, 0.2f);
    glutSolidSphere(1.0f, 16, 16);
    glPopMatrix();

    // Hoa
    glColor3f(0.8f, 0.2f, 0.2f);  // Đỏ
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
    glColor3f(0.6f, 0.3f, 0.3f);  // Đỏ nhạt
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
    glTranslatef(0.6f, 0.75f, -0.4f);  // Trên bàn
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
    if (light5_enabled) {
        GLfloat emission[] = { 1.0f, 0.9f, 0.5f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glColor3f(1.0f, 0.9f, 0.5f);  // Vàng sáng
    } else {
        GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glColor3f(0.9f, 0.9f, 0.8f);  // Màu chụp đèn khi tắt
    }
    glPushMatrix();
    glTranslatef(0.6f, 0.95f, -0.4f);
    glScalef(0.2f, 0.2f, 0.2f);
    glutSolidCone(1.0f, 1.0f, 16, 16);
    glPopMatrix();

    // Đặt lại thuộc tính vật liệu
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
}

// Hàm vẽ đèn tường
void drawWallLights() {
    // Đèn tường trái
    if (light7_enabled) {
        GLfloat emission[] = { 0.6f, 0.6f, 0.6f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, emission);
        glColor3f(0.8f, 0.8f, 0.8f);  // Trắng sáng
    } else {
        GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
        glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
        glColor3f(0.8f, 0.8f, 0.8f);  // Xám nhạt
    }
    glPushMatrix();
    glTranslatef(-4.8f, 2.5f, -2.0f);
    glScalef(0.2f, 0.4f, 0.1f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Đặt lại thuộc tính vật liệu
    GLfloat no_emission[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    glMaterialfv(GL_FRONT, GL_EMISSION, no_emission);
    GLfloat default_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    glMaterialfv(GL_FRONT, GL_DIFFUSE, default_diffuse);
}

// Hàm vẽ văn bản trên màn hình
void drawText(float x, float y, const char* string) {
    glColor3f(1.0f, 1.0f, 1.0f); // Màu trắng
    glRasterPos2f(x, y);
    for (const char* c = string; *c != '\0'; c++) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *c);
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(cameraX, cameraY, cameraZ,
              cameraX + sin(cameraAngle) * cos(cameraPitch),
              cameraY + sin(cameraPitch),
              cameraZ - cos(cameraAngle) * cos(cameraPitch),
              0.0f, 1.0f, 0.0f);

    // Cap nhat anh sang TV
    GLfloat tv_diffuse[] = { tv_brightness, tv_brightness, tv_brightness, 1.0f };
    glLightfv(GL_LIGHT6, GL_DIFFUSE, tv_diffuse);

    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
    glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
    glLightfv(GL_LIGHT3, GL_POSITION, light3_position);
    glLightfv(GL_LIGHT4, GL_POSITION, light4_position);
    glLightfv(GL_LIGHT5, GL_POSITION, lamp_light_position);
    glLightfv(GL_LIGHT6, GL_POSITION, tv_light_position);
    glLightfv(GL_LIGHT7, GL_POSITION, wall_light_position);

    drawFloor();
    drawCeiling();
    drawCeilingLights();
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

    // Chuyen sang che do phoi canh 2D de ve van ban
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, glutGet(GLUT_WINDOW_WIDTH), 0, glutGet(GLUT_WINDOW_HEIGHT));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Tat anh sang va kiem tra do sau de ve van ban
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    // Ve huong dan dieu khien o goc duoi trai
    int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
    drawText(10, windowHeight - 20, "Dieu khien:");
    drawText(10, windowHeight - 40, "W: Di chuyen toi");
    drawText(10, windowHeight - 60, "S: Di chuyen lui");
    drawText(10, windowHeight - 80, "A: Xoay trai");
    drawText(10, windowHeight - 100, "D: Xoay phai");
    drawText(10, windowHeight - 120, "Q: Di len");
    drawText(10, windowHeight - 140, "E: Di xuong");
    drawText(10, windowHeight - 160, "Z: Nhin len");
    drawText(10, windowHeight - 180, "C: Nhin xuong");
    drawText(10, windowHeight - 200, "X: Tang toc do");
    drawText(10, windowHeight - 220, "V: Giam toc do");
    drawText(10, windowHeight - 240, "0: Bat tat tat ca den");
    drawText(10, windowHeight - 260, "1: Bat tat den tran trung tam");
    drawText(10, windowHeight - 280, "2: Bat tat den goc tren trai");
    drawText(10, windowHeight - 300, "3: Bat tat den goc tren phai");
    drawText(10, windowHeight - 320, "4: Bat tat den goc duoi trai");
    drawText(10, windowHeight - 340, "5: Bat tat den goc duoi phai");
    drawText(10, windowHeight - 360, "6: Bat tat den ban");
    drawText(10, windowHeight - 380, "7: Bat tat den TV");
    drawText(10, windowHeight - 400, "8: Bat tat den tuong");

    // Khoi phuc che do phoi canh 3D va trang thai
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glutSwapBuffers();
}

// Hàm xử lý sự kiện bàn phím
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 'w': cameraX += sin(cameraAngle) * cameraSpeed; cameraZ -= cos(cameraAngle) * cameraSpeed; break;  // Di chuyển tới
        case 's': cameraX -= sin(cameraAngle) * cameraSpeed; cameraZ += cos(cameraAngle) * cameraSpeed; break;  // Di chuyển lui
        case 'a': cameraAngle -= rotationSpeed; break;  // Xoay trái
        case 'd': cameraAngle += rotationSpeed; break;  // Xoay phải
        case 'q': cameraY += cameraSpeed; break;  // Di lên
        case 'e': cameraY -= cameraSpeed; break;  // Di xuống
        case 'z': cameraPitch += rotationSpeed; break;  // Nhìn lên
        case 'c': cameraPitch -= rotationSpeed; break;  // Nhìn xuống
        case 'x': cameraSpeed *= 1.1f; break;  // Tăng tốc độ
        case 'v': cameraSpeed *= 0.9f; break;  // Giảm tốc độ
        case '0': toggleAllLights(); if (light6_enabled) glutTimerFunc(0, updateTVBrightness, 0); break;  // Bật/tắt tất cả đèn
        case '1': toggleLight(0, light0_enabled); break;  // Bật/tắt đèn trần trung tâm
        case '2': toggleLight(1, light1_enabled); break;  // Bật/tắt đèn góc trên trái
        case '3': toggleLight(2, light2_enabled); break;  // Bật/tắt đèn góc trên phải
        case '4': toggleLight(3, light3_enabled); break;  // Bật/tắt đèn góc dưới trái
        case '5': toggleLight(4, light4_enabled); break;  // Bật/tắt đèn góc dưới phải
        case '6': toggleLight(5, light5_enabled); break;  // Bật/tắt đèn bàn
        case '7': toggleLight(6, light6_enabled); if (light6_enabled) glutTimerFunc(0, updateTVBrightness, 0); break;  // Bật/tắt đèn TV
        case '8': toggleLight(7, light7_enabled); break;  // Bật/tắt đèn tường
    }
    glutPostRedisplay();
}

// Hàm xử lý thay đổi kích thước cửa sổ
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / h, 0.1f, 100.0f);  // Góc nhìn 45 độ
    glMatrixMode(GL_MODELVIEW);
}

// Hàm main
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL);  // Chế độ hiển thị kép, RGB, độ sâu và stencil
    glutInitWindowSize(800, 600);  // Kích thước cửa sổ
    glutCreateWindow("Phong khach 3D");  // Tên cửa sổ

    init();  // Khởi tạo OpenGL

    glutDisplayFunc(display);  // Hàm vẽ
    glutReshapeFunc(reshape);  // Hàm xử lý thay đổi kích thước
    glutKeyboardFunc(keyboard);  // Hàm xử lý bàn phím
    glutMainLoop();  // Vòng lặp chính
    return 0;
}
