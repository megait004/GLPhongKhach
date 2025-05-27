#include <GL/glut.h>
#include <cmath>

// Các biến điều khiển camera
float cameraX = 0.0f;      // Vị trí X của camera
float cameraY = 2.0f;      // Vị trí Y của camera (chiều cao)
float cameraZ = 5.0f;      // Vị trí Z của camera
float cameraAngle = 0.0f;  // Góc xoay camera theo trục Y
float cameraPitch = 0.0f;  // Góc nghiêng camera lên/xuống
float cameraSpeed = 0.1f;  // Tốc độ di chuyển camera
float rotationSpeed = 0.05f; // Tốc độ xoay camera

// Các thông số ánh sáng
GLfloat light_position[] = { 2.0f, 5.0f, 2.0f, 1.0f };  // Vị trí đèn chính
GLfloat light_ambient[] = { 0.3f, 0.3f, 0.3f, 1.0f };  // Ánh sáng môi trường
GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };  // Ánh sáng khuếch tán

// Thông số ánh sáng cho đèn bàn (màu vàng)
GLfloat lamp_light_ambient[] = { 0.2f, 0.2f, 0.1f, 1.0f };  // Ánh sáng môi trường màu vàng nhạt
GLfloat lamp_light_diffuse[] = { 1.0f, 0.9f, 0.5f, 1.0f };  // Ánh sáng khuếch tán màu vàng ấm

// Thông số ánh sáng cho đèn tường
GLfloat wall_light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };  // Ánh sáng môi trường
GLfloat wall_light_diffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };  // Ánh sáng khuếch tán

// Vị trí các đèn
GLfloat lamp_light_position[] = { 1.5f, 1.1f, -3.5f, 1.0f };    // Vị trí đèn bàn
GLfloat tv_light_position[] = { 0.0f, 2.0f, -4.8f, 1.0f };      // Vị trí đèn TV
GLfloat wall_light1_position[] = { -4.8f, 2.5f, 0.0f, 1.0f };   // Vị trí đèn tường trái
GLfloat wall_light2_position[] = { 4.8f, 2.5f, 0.0f, 1.0f };    // Vị trí đèn tường phải

// Trạng thái các đèn
bool light0_enabled = true;   // Đèn chính (mặc định bật)
bool light1_enabled = false;  // Đèn bàn (mặc định tắt)
bool light2_enabled = false;  // Đèn TV (mặc định tắt)
bool light3_enabled = false;  // Đèn tường trái (mặc định tắt)
bool light4_enabled = false;  // Đèn tường phải (mặc định tắt)

// Hàm bật/tắt đèn
void toggleLight(int lightNum, bool& lightState) {
    lightState = !lightState;
    if (lightState) {
        glEnable(GL_LIGHT0 + lightNum);
    } else {
        glDisable(GL_LIGHT0 + lightNum);
    }
}

// Hàm bật/tắt tất cả đèn
void toggleAllLights() {
    bool newState = !light0_enabled;  // Lấy trạng thái ngược của đèn chính
    light0_enabled = newState;
    light1_enabled = newState;
    light2_enabled = newState;
    light3_enabled = newState;
    light4_enabled = newState;

    if (newState) {
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHT2);
        glEnable(GL_LIGHT3);
        glEnable(GL_LIGHT4);
    } else {
        glDisable(GL_LIGHT0);
        glDisable(GL_LIGHT1);
        glDisable(GL_LIGHT2);
        glDisable(GL_LIGHT3);
        glDisable(GL_LIGHT4);
    }
}

void init() {
    glEnable(GL_DEPTH_TEST);    // Bật kiểm tra độ sâu
    glEnable(GL_LIGHTING);      // Bật tính năng ánh sáng
    glEnable(GL_LIGHT0);        // Bật đèn chính
    glEnable(GL_COLOR_MATERIAL);// Cho phép thay đổi màu vật liệu

    // Thiết lập đèn chính (LIGHT0)
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    // Thiết lập đèn bàn (LIGHT1)
    glEnable(GL_LIGHT1);
    glLightfv(GL_LIGHT1, GL_POSITION, lamp_light_position);
    glLightfv(GL_LIGHT1, GL_AMBIENT, lamp_light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, lamp_light_diffuse);
    glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.1f);

    // Thiết lập đèn TV (LIGHT2)
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2, GL_POSITION, tv_light_position);
    glLightfv(GL_LIGHT2, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT2, GL_DIFFUSE, light_diffuse);
    glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.1f);

    // Thiết lập đèn tường trái (LIGHT3)
    glEnable(GL_LIGHT3);
    glLightfv(GL_LIGHT3, GL_POSITION, wall_light1_position);
    glLightfv(GL_LIGHT3, GL_AMBIENT, wall_light_ambient);
    glLightfv(GL_LIGHT3, GL_DIFFUSE, wall_light_diffuse);
    glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.1f);

    // Thiết lập đèn tường phải (LIGHT4)
    glEnable(GL_LIGHT4);
    glLightfv(GL_LIGHT4, GL_POSITION, wall_light2_position);
    glLightfv(GL_LIGHT4, GL_AMBIENT, wall_light_ambient);
    glLightfv(GL_LIGHT4, GL_DIFFUSE, wall_light_diffuse);
    glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 0.5f);
    glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.1f);
}

void drawFloor() {
    glColor3f(0.8f, 0.8f, 0.8f);  // Màu xám nhạt
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);  // Vector pháp tuyến hướng lên
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, 5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glEnd();
}

void drawWalls() {
    // Tường sau
    glColor3f(0.9f, 0.9f, 0.9f);  // Màu trắng nhạt
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 0.0f, 1.0f);  // Vector pháp tuyến hướng ra ngoài
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(5.0f, 4.0f, -5.0f);
    glVertex3f(5.0f, 0.0f, -5.0f);
    glEnd();

    // Tường bên
    glBegin(GL_QUADS);
    glNormal3f(1.0f, 0.0f, 0.0f);  // Vector pháp tuyến hướng sang phải
    glVertex3f(-5.0f, 0.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, -5.0f);
    glVertex3f(-5.0f, 4.0f, 5.0f);
    glVertex3f(-5.0f, 0.0f, 5.0f);
    glEnd();
}

void drawSofa() {
    glColor3f(0.4f, 0.2f, 0.1f);  // Màu nâu

    // Phần đế ghế
    glPushMatrix();
    glTranslatef(-2.0f, 0.4f, 3.0f);  // Di chuyển đến vị trí bên trái
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Xoay 180 độ để hướng về TV
    glScalef(2.0f, 0.4f, 1.0f);  // Kích thước ghế
    glutSolidCube(1.0f);
    glPopMatrix();

    // Phần lưng ghế
    glPushMatrix();
    glTranslatef(-2.0f, 0.8f, 3.5f);  // Đặt lưng ghế phía sau đế
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawSofa1() {
    glColor3f(0.4f, 0.2f, 0.1f);  // Màu nâu

    // Phần đế ghế
    glPushMatrix();
    glTranslatef(2.0f, 0.4f, 3.0f);  // Di chuyển đến vị trí bên phải
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);  // Xoay 180 độ để hướng về TV
    glScalef(2.0f, 0.4f, 1.0f);
    glutSolidCube(1.0f);
    glPopMatrix();

    // Phần lưng ghế
    glPushMatrix();
    glTranslatef(2.0f, 0.8f, 3.5f);  // Đặt lưng ghế phía sau đế
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    glScalef(2.0f, 0.8f, 0.2f);
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawTable() {
    // Mặt bàn
    glColor3f(0.6f, 0.4f, 0.2f);  // Màu nâu gỗ
    glPushMatrix();
    glTranslatef(0.0f, 0.7f, -2.0f);  // Vị trí mặt bàn
    glScalef(1.2f, 0.1f, 0.8f);  // Kích thước mặt bàn
    glutSolidCube(1.0f);
    glPopMatrix();

    // Chân bàn
    glColor3f(0.3f, 0.2f, 0.1f);  // Màu nâu đậm
    for(int i = -1; i <= 1; i += 2) {
        for(int j = -1; j <= 1; j += 2) {
            glPushMatrix();
            glTranslatef(0.0f + i*0.5f, 0.35f, -2.0f + j*0.3f);  // Vị trí 4 chân bàn
            glScalef(0.1f, 0.7f, 0.1f);  // Kích thước chân bàn
            glutSolidCube(1.0f);
            glPopMatrix();
        }
    }
}

void drawTV() {
    // Màn hình TV
    glColor3f(0.1f, 0.1f, 0.1f);  // Màu đen
    glPushMatrix();
    glTranslatef(0.0f, 2.0f, -4.9f);  // Vị trí màn hình
    glScalef(1.5f, 0.9f, 0.1f);  // Kích thước màn hình
    glutSolidCube(1.0f);
    glPopMatrix();

    // Chân đế TV
    glColor3f(0.3f, 0.3f, 0.3f);  // Màu xám
    glPushMatrix();
    glTranslatef(0.0f, 1.5f, -4.5f);  // Vị trí chân đế
    glScalef(1.6f, 0.1f, 0.4f);  // Kích thước chân đế
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawCurtains() {
    // Thanh treo rèm
    glColor3f(0.3f, 0.3f, 0.3f);  // Màu xám
    glPushMatrix();
    glTranslatef(0.0f, 3.8f, -4.9f);  // Vị trí thanh treo
    glScalef(3.0f, 0.05f, 0.05f);  // Kích thước thanh treo
    glutSolidCube(1.0f);
    glPopMatrix();

    // Rèm cửa
    glColor3f(0.8f, 0.8f, 0.9f);  // Màu trắng nhạt
    for(float x = -2.5f; x <= 2.5f; x += 0.2f) {
        glBegin(GL_QUADS);
        glNormal3f(0.0f, 0.0f, 1.0f);
        glVertex3f(x, 3.7f, -4.9f);
        glVertex3f(x, 0.0f, -4.9f);
        glVertex3f(x + 0.1f, 0.0f, -4.9f);
        glVertex3f(x + 0.1f, 3.7f, -4.9f);
        glEnd();
    }
}

void drawWallPainting() {
    // Khung tranh
    glColor3f(0.4f, 0.3f, 0.2f);  // Màu nâu
    glPushMatrix();
    glTranslatef(-3.0f, 2.5f, -4.9f);  // Vị trí khung tranh
    glScalef(0.8f, 0.6f, 0.05f);  // Kích thước khung tranh
    glutSolidCube(1.0f);
    glPopMatrix();

    // Nội dung tranh
    glColor3f(0.8f, 0.6f, 0.4f);  // Màu vàng nhạt
    glPushMatrix();
    glTranslatef(-3.0f, 2.5f, -4.85f);  // Vị trí nội dung tranh
    glScalef(0.7f, 0.5f, 0.01f);  // Kích thước nội dung tranh
    glutSolidCube(1.0f);
    glPopMatrix();
}

void drawVase() {
    // Thân lọ
    glColor3f(0.2f, 0.4f, 0.6f);  // Màu xanh dương
    glPushMatrix();
    glTranslatef(2.0f, 0.5f, -2.0f);  // Vị trí lọ hoa
    glScalef(0.2f, 0.4f, 0.2f);  // Kích thước lọ
    glutSolidSphere(1.0f, 16, 16);
    glPopMatrix();

    // Hoa trong lọ
    glColor3f(0.8f, 0.2f, 0.2f);  // Màu đỏ
    for(int i = 0; i < 5; i++) {
        float angle = i * 72.0f * 3.14159f / 180.0f;  // Góc xoay cho mỗi bông hoa
        glPushMatrix();
        glTranslatef(2.0f + 0.1f * cos(angle), 0.9f, -2.0f + 0.1f * sin(angle));
        glScalef(0.05f, 0.2f, 0.05f);  // Kích thước bông hoa
        glutSolidSphere(1.0f, 8, 8);
        glPopMatrix();
    }
}

void drawCarpet() {
    // Thảm chính
    glColor3f(0.6f, 0.3f, 0.3f);  // Màu đỏ nhạt
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-2.0f, 0.01f, -3.0f);
    glVertex3f(2.0f, 0.01f, -3.0f);
    glVertex3f(2.0f, 0.01f, -1.0f);
    glVertex3f(-2.0f, 0.01f, -1.0f);
    glEnd();

    // Họa tiết thảm
    glColor3f(0.7f, 0.4f, 0.4f);  // Màu đỏ đậm hơn
    glBegin(GL_QUADS);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-1.8f, 0.02f, -2.8f);
    glVertex3f(1.8f, 0.02f, -2.8f);
    glVertex3f(1.8f, 0.02f, -1.2f);
    glVertex3f(-1.8f, 0.02f, -1.2f);
    glEnd();
}

void drawLamp() {
    // Đế đèn
    glColor3f(0.3f, 0.3f, 0.3f);  // Màu xám
    glPushMatrix();
    glTranslatef(1.5f, 0.1f, -3.5f);  // Vị trí đế đèn
    glScalef(0.2f, 0.1f, 0.2f);  // Kích thước đế đèn
    glutSolidCube(1.0f);
    glPopMatrix();

    // Cột đèn
    glPushMatrix();
    glTranslatef(1.5f, 0.6f, -3.5f);  // Vị trí cột đèn
    glScalef(0.05f, 0.5f, 0.05f);  // Kích thước cột đèn
    glutSolidCube(1.0f);
    glPopMatrix();

    // Chụp đèn
    glColor3f(0.9f, 0.9f, 0.8f);  // Màu trắng ngà
    glPushMatrix();
    glTranslatef(1.5f, 1.1f, -3.5f);  // Vị trí chụp đèn
    glScalef(0.3f, 0.2f, 0.3f);  // Kích thước chụp đèn
    glutSolidCone(1.0f, 1.0f, 16, 16);
    glPopMatrix();
}

void drawWallLights() {
    // Đèn tường trái
    glColor3f(0.8f, 0.8f, 0.8f);  // Màu trắng
    glPushMatrix();
    glTranslatef(-4.8f, 2.5f, 0.0f);  // Vị trí đèn tường trái
    glScalef(0.2f, 0.4f, 0.1f);  // Kích thước đèn
    glutSolidCube(1.0f);
    glPopMatrix();

    // Đèn tường phải
    glPushMatrix();
    glTranslatef(4.8f, 2.5f, 0.0f);  // Vị trí đèn tường phải
    glScalef(0.2f, 0.4f, 0.1f);  // Kích thước đèn
    glutSolidCube(1.0f);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Xóa màn hình
    glLoadIdentity();  // Reset ma trận

    // Thiết lập camera với góc nhìn
    gluLookAt(cameraX, cameraY, cameraZ,
              cameraX + sin(cameraAngle) * cos(cameraPitch),
              cameraY + sin(cameraPitch),
              cameraZ - cos(cameraAngle) * cos(cameraPitch),
              0.0f, 1.0f, 0.0f);

    // Vẽ các thành phần trong phòng
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
    drawWallLights();  // Thêm vẽ đèn tường

    glutSwapBuffers();  // Hoán đổi buffer để hiển thị
}

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case 'a': cameraAngle -= rotationSpeed; break;  // Xoay camera sang trái
        case 'd': cameraAngle += rotationSpeed; break;  // Xoay camera sang phải
        case 'w':  // Di chuyển camera tiến lên
            cameraX += sin(cameraAngle) * cameraSpeed;
            cameraZ -= cos(cameraAngle) * cameraSpeed;
            break;
        case 's':  // Di chuyển camera lùi lại
            cameraX -= sin(cameraAngle) * cameraSpeed;
            cameraZ += cos(cameraAngle) * cameraSpeed;
            break;
        case 'q': cameraY += cameraSpeed; break;  // Di chuyển camera lên
        case 'e': cameraY -= cameraSpeed; break;  // Di chuyển camera xuống
        case 'z': cameraPitch += rotationSpeed; break;  // Ngước camera lên
        case 'c': cameraPitch -= rotationSpeed; break;  // Cúi camera xuống
        case 'x': cameraSpeed *= 1.1f; break;  // Tăng tốc độ di chuyển
        case 'v': cameraSpeed *= 0.9f; break;  // Giảm tốc độ di chuyển

        // Điều khiển đèn
        case '1': toggleLight(0, light0_enabled); break;  // Bật/tắt đèn chính
        case '2': toggleLight(1, light1_enabled); break;  // Bật/tắt đèn bàn
        case '3': toggleLight(2, light2_enabled); break;  // Bật/tắt đèn TV
        case '4': toggleLight(3, light3_enabled); break;  // Bật/tắt đèn tường trái
        case '5': toggleLight(4, light4_enabled); break;  // Bật/tắt đèn tường phải
        case '0': toggleAllLights(); break;  // Bật/tắt tất cả đèn
    }
    glutPostRedisplay();  // Yêu cầu vẽ lại cảnh
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);  // Thiết lập vùng nhìn
    glMatrixMode(GL_PROJECTION);  // Chọn ma trận chiếu
    glLoadIdentity();  // Reset ma trận
    gluPerspective(45.0f, (float)w/h, 0.1f, 100.0f);  // Thiết lập phép chiếu phối cảnh
    glMatrixMode(GL_MODELVIEW);  // Chọn ma trận modelview
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);  // Khởi tạo GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  // Thiết lập chế độ hiển thị
    glutInitWindowSize(800, 600);  // Kích thước cửa sổ
    glutCreateWindow("3D Living Room");  // Tạo cửa sổ

    init();  // Khởi tạo các thông số

    glutDisplayFunc(display);  // Đăng ký hàm vẽ
    glutReshapeFunc(reshape);  // Đăng ký hàm xử lý thay đổi kích thước
    glutKeyboardFunc(keyboard);  // Đăng ký hàm xử lý bàn phím
    glutMainLoop();  // Vòng lặp chính
    return 0;
}
