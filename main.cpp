
#include <Arduino.h>
#include <M5UnitGLASS2.h>
#include <M5Unified.h>
#include <M5GFX.h>

#define DINO_WIDTH 25
#define DINO_HEIGHT 26
#define DINO_INIT_X 10 // Dino initial spawn location
#define DINO_INIT_Y 29 // Dino initial spawn location
#define BASE_LINE_X 0
#define BASE_LINE_Y 54
#define BASE_LINE_X1 127
#define BASE_LINE_Y1 54
#define TREE1_WIDTH 11
#define TREE1_HEIGHT 23
#define TREE2_WIDTH 22
#define TREE2_HEIGHT 23
#define TREE_Y 35
#define JUMP_PIXEL 22 // Number of pixel dino will jump

M5UnitGLASS2 display(13, 15, 400000); // SDA, SCL, FREQ  //


static M5Canvas canvas(&display);

static const unsigned char PROGMEM dino1[]={
  // 'dino', 25x26px
0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xfe, 0x00, 0x00, 0x06, 0xff, 0x00, 0x00, 0x0e, 0xff, 0x00, 
0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xff, 0x00, 0x00, 0x0f, 0xc0, 0x00, 
0x00, 0x0f, 0xfc, 0x00, 0x40, 0x0f, 0xc0, 0x00, 0x40, 0x1f, 0x80, 0x00, 0x40, 0x7f, 0x80, 0x00, 
0x60, 0xff, 0xe0, 0x00, 0x71, 0xff, 0xa0, 0x00, 0x7f, 0xff, 0x80, 0x00, 0x7f, 0xff, 0x80, 0x00, 
0x7f, 0xff, 0x80, 0x00, 0x3f, 0xff, 0x00, 0x00, 0x1f, 0xff, 0x00, 0x00, 0x0f, 0xfe, 0x00, 0x00, 
0x03, 0xfc, 0x00, 0x00, 0x01, 0xdc, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 0x01, 0x8c, 0x00, 0x00, 
0x01, 0x0c, 0x00, 0x00, 0x01, 0x8e, 0x00, 0x00
};

static const unsigned char PROGMEM tree1[]={
  // 'tree1', 11x23px
0x1e, 0x00, 0x1f, 0x00, 0x1f, 0x40, 0x1f, 0xe0, 0x1f, 0xe0, 0xdf, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 
0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xe0, 0xff, 0xc0, 0xff, 0x00, 0xff, 0x00, 0x7f, 0x00, 
0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00, 0x1f, 0x00
};

static const unsigned char PROGMEM tree2[]={
  // 'tree2', 22x23px
0x1e, 0x01, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x4f, 0xe8, 0x1f, 0xff, 0xfc, 0x1f, 0xff, 0xfc, 0xdf, 
0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 0xfc, 0xff, 0xff, 
0xfc, 0xff, 0xef, 0xfc, 0xff, 0x83, 0xfc, 0xff, 0x03, 0xfc, 0xff, 0x03, 0xf8, 0x7f, 0x03, 0xe0, 
0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 0x03, 0xe0, 0x1f, 
0x03, 0xe0, 0x1f, 0x03, 0xe0
};

void introMessage(){
  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10,5);
  display.println("Dino Game");
  

  display.setTextSize(1);
  
  display.setCursor(5,40);
  display.println("Push Button To Play ");

  display.display();
}

// Move dino function
void moveDino(int16_t *y, int type=0){
  canvas.drawBitmap(DINO_INIT_X, *y, dino1, DINO_WIDTH, DINO_HEIGHT, WHITE);
}

// Move tree funciton
void moveTree(int16_t *x, int type=0){
  if(type==0){
    canvas.drawBitmap(*x, TREE_Y, tree1, TREE1_WIDTH, TREE1_HEIGHT, WHITE);
  }
  else if(type==1){
    canvas.drawBitmap(*x, TREE_Y, tree2, TREE2_WIDTH, TREE2_HEIGHT, WHITE);
  }
  
}

// Game over display with score
void gameOver(int score=0){
  display.clearDisplay();

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(10,5);
  display.println("Game Over");
  

  display.setTextSize(1);
  display.setCursor(25,27);
  display.print("Score: ");
  display.print(score);
  
  display.setCursor(20,39);
  display.println("Push Button ");
  display.setCursor(20,49);
  display.println(" To Play Again");

  display.display();
  M5.Speaker.tone(1000, 100);
  delay(100);
  M5.Speaker.tone(800, 100);
  delay(400);

}

// バーグラフを表示する関数
void displayLifeBar(int lives) {
  int barWidth = 40;  // バーの横幅
  int barHeight = 2;  // バーの高さ
  int maxLives = 20;  // 最大ライフ
  int barX = 67;      // バーの表示位置X（スコアの下）
  int barY = 20;      // バーの表示位置Y（ライフの下）

  // ライフに応じたバーの幅を計算
  int filledBarWidth = (barWidth * lives) / maxLives;

  // 空のバー（背景）
  canvas.fillRect(barX, barY, barWidth, barHeight, BLACK); // 背景を黒で描画

  // 埋められたバー（ライフに応じた部分）
  canvas.fillRect(barX, barY, filledBarWidth, barHeight, WHITE); // ライフに応じたバーを白で描画
}

// Display score while running the game
void displayScoreAndLives(int score,int lives){
  canvas.setTextSize(1);
  canvas.setCursor(67,1);
  canvas.print("Score: ");
  canvas.print(score);
  canvas.setCursor(67, 11); // スコアの下にライフを表示
  canvas.print("life: ");
  canvas.print(lives);
  displayLifeBar(lives);  // ライフバーを表示
}


// Main play function
void play() {
    int16_t tree_x = 127;
    int16_t tree1_x = 195;
    int tree_type = random(0, 2);
    int tree_type1 = random(0, 2);
    int16_t dino_y = DINO_INIT_Y;
    int jump = 0;
    int score = 0;
    int lives = 20;  // ライフは50からスタート

    while (lives > 0) {
        canvas.clearDisplay();
        M5.update();

        if (M5.BtnA.wasPressed()) {
            if (jump == 0) jump = 1;
        }

        // 恐竜のジャンプロジック
        if (jump == 1) {
            dino_y--;
            if (dino_y == (DINO_INIT_Y - JUMP_PIXEL)) {
                jump = 2;
                score++;
            }
        } else if (jump == 2) {
            dino_y++;
            if (dino_y == DINO_INIT_Y) {
                jump = 0;
            }
        }

        // 木との衝突判定
           if ((tree_x <= (DINO_INIT_X + DINO_WIDTH) && tree_x >= (DINO_INIT_X + (DINO_WIDTH / 2))) || 
            (tree1_x <= (DINO_INIT_X + DINO_WIDTH) && tree1_x >= (DINO_INIT_X + (DINO_WIDTH / 2)))) {
            if (dino_y >= (DINO_INIT_Y - 3)) {
                lives--;  // 衝突したらライフを減らす
                if (lives > 0) {
                    // ライフが残っている場合はゲーム続行
                    delay(15);  // 衝突が見えるように一時停止
                } else {
                    break;  // ライフがなくなったらゲーム終了
                }
            }
        }

        // スコアとライフの表示
        displayScoreAndLives(score, lives);
        moveTree(&tree_x, tree_type);
        moveTree(&tree1_x, tree_type1);
        moveDino(&dino_y);
        canvas.drawLine(0, 54, 127, 54, WHITE);

        // 木の移動ロジック
        tree_x--;
        tree1_x--;
        if (tree_x == 0) {
            tree_x = 127;
            tree_type = random(0, 1);
        }
        if (tree1_x == 0) {
            tree1_x = 195;
            tree_type1 = random(0, 1);
        }

        canvas.pushSprite(0, 0);
    }

    gameOver(score);  // ライフがなくなったらゲームオーバーを表示
}


void setup(void){

  auto cfg = M5.config();

 //   cfg.external_spk = true;    /// use external speaker (SPK HAT / ATOMIC SPK)
//cfg.external_spk_detail.omit_atomic_spk = true; // exclude ATOMIC SPK
//cfg.external_spk_detail.omit_spk_hat    = true; // exclude SPK HAT

  M5.begin(cfg);

  M5.Speaker.begin();
 
  M5.Speaker.setVolume(200);

  M5.Displays(0).setBrightness(255);
  M5.Displays(0).setRotation(1);
  M5.Displays(0).setColorDepth(1);

  display.begin(); // displayの初期化
  display.init();
  display.setRotation(1); // displayの向きを指定

 // canvas.setFont(&fonts::Font4); // フォント指定
  canvas.setTextSize(1); // 
  canvas.setTextWrap(false);  // 自動改行機能［true = 使用，false = 未使用］
  canvas.createSprite(128,64);

  M5.Displays(0).setTextColor(TFT_WHITE,TFT_BLACK);

  M5.Display.clear();

  introMessage();
  // Run game in loop


}

void loop(void)
{

  M5.update();

    if(M5.BtnA.wasPressed()){
      M5.Speaker.tone(1000, 100);
      delay(200);
      play();
     }

  
}



