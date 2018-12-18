
/* <><>-FILLER SHOOTING-<><> */

#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1); //DIN, CLK, LOAD, nr. DRIVER
// pin 12 conectat la the MAX7219 pin 1
// pin 11 conectat la CLK pin 13
// pin 10 conectat la LOAD pin 12
// 1, deoarece folosim 1 MAX7219

#define SW_PIN 2 // pin digital ce retine valoarea la apasare de buton
#define X_PIN A0 // pin analog conectat la axa OX
#define Y_PIN A1 // pin analog conectat la axa OY
int switchValue, xValue, yValue; //aici se retin valorile primite input

int initial = 0; //switch intre meniu si jocul in sine
unsigned long motionStart = millis();
unsigned long motionEnd = motionStart + 180; //timer-e pentru miscarea tancului
unsigned long motionPresent;
int shipLeft = 2;
int shipRight = 4;
int shipCannon = (shipLeft + shipRight) / 2; //coordonatele tancului
int fillLevel = 0; //cel mai de jos rand generat
int empty[7]; //retine nr de casute goale de pe fiecare rand
int difficultyValue;
unsigned long difficultyTimer; //timere pentru setarea dificultatii
unsigned long difficultyStart = millis();
unsigned long difficultyEnd = difficultyStart + 350;
int scoreCounter = 0;
int matrix[8][8]={ {0,0,0,1,1,0,0,0},
                    {0,0,1,0,0,1,0,0},
                    {0,1,0,0,0,0,1,0},
                    {1,0,0,1,1,0,0,1},
                    {1,0,0,1,1,0,0,1},
                    {0,1,0,0,0,0,1,0},
                    {0,0,1,0,0,1,0,0},
                    {0,0,0,1,1,0,0,0},
                  };
int speedCount = 0;
unsigned long interval = 3500; //intervalul in milisecunde la care apare un rand nou generat
unsigned long generatorStart = millis();
unsigned long generatorEnd = generatorStart + interval; //timer-e pentru generarea formelor
unsigned long generatorPresent;

void startAnimation()
{
    for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
                lc.setLed(0, row, col, matrix[row][col]);
            delay(150);
        }
        for (int row = 0; row < 8; row++)
            for (int col = 0; col < 8; col++)
                matrix[row][col] = 0;
        for (int row = 0; row < 8; row++)
        {
            for (int col = 0; col < 8; col++)
                lc.setLed(0, row, col, matrix[row][col]);
            delay(150);
        }
}

void increaseDifficulty()
{
    if (difficultyTimer >= difficultyEnd)
    {
        if (difficultyValue <= 4)
        {
            difficultyValue += 1;
            lc.setLed(0, 7, difficultyValue, 1);
        }
        difficultyStart = millis();
        difficultyEnd = difficultyStart + 350;
    }
}

void decreaseDifficulty()
{
    if (difficultyTimer >= difficultyEnd)
    {
        if (difficultyValue >= 4)
        {
            lc.setLed(0, 7, difficultyValue, 0);
            difficultyValue -= 1;
        }
        difficultyStart = millis();
        difficultyEnd = difficultyStart + 350;
    }
}

void moveLeft()
{
    if (shipLeft > 0)
    {
        matrix[7][shipRight] = 0;
        shipLeft -= 1;
        shipRight -= 1;
        matrix[7][shipLeft] = 1;
        shipCannon -= 1;
        matrix[6][shipCannon] = 1;
        matrix[6][shipCannon + 1] = 0;
    }
}

void moveRight()
{
    if (shipRight < 7)
    {
        matrix[7][shipLeft] = 0;
        shipLeft += 1;
        shipRight += 1;
        matrix[7][shipRight] = 1;
        shipCannon += 1;
        matrix[6][shipCannon] = 1;
        matrix[6][shipCannon - 1] = 0;
    }
}

void explosionAnimation()
{
    matrix[5][shipCannon - 1] = 0;
    matrix[5][shipCannon] = 0;
    matrix[5][shipCannon + 1] = 0;
    matrix[6][shipCannon - 1] = 0;
    matrix[6][shipCannon + 1] = 0;
    matrix[7][shipCannon - 1] = 0;
    matrix[7][shipCannon] = 0;
    matrix[7][shipCannon + 1] = 0;
    matrix[6][shipCannon] = 1;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            lc.setLed(0, i, j, matrix[i][j]);
    delay(180);
    matrix[5][shipCannon - 1] = 1;
    matrix[5][shipCannon] = 1;
    matrix[5][shipCannon + 1] = 1;
    matrix[6][shipCannon - 1] = 1;
    matrix[6][shipCannon + 1] = 1;
    matrix[7][shipCannon - 1] = 1;
    matrix[7][shipCannon] = 1;
    matrix[7][shipCannon + 1] = 1;
    matrix[6][shipCannon] = 0;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            lc.setLed(0, i, j, matrix[i][j]);
    delay(600);
}

void generateShape()
{
    int v[8];
    for (int i = 0; i <= 7; i++)
        v[i] = 1;
    int emptySlots = difficultyValue - 1;
    for (int i = 0; i < emptySlots; i++)
    {
        int ok = 0;
        int positioning;
        while (ok == 0)
        {
            positioning = random(1,7); //se genereaza random golurile in randul care urmeaza a fi pus pe matrix
            if (v[positioning] == 1)
            {
                ok = 1;
                v[positioning] = 0;
            }
        }
    }
    if (fillLevel == 1) //se introduce randul nou generat pe matrix
    {
        for (int i = 0; i < 8; i++)
            matrix[0][i] = v[i];
        empty[0] = emptySlots;
    }
    else //cazul in care sunt mai multe randuri deja generate
    {
        for (int i = fillLevel - 1; i > 0; i--)
            for (int j = 0; j < 8; j++)
                matrix[i][j] = matrix[i - 1][j];
        for (int i = 0; i < 8; i++)
            matrix[0][i] = v[i];
        for (int i = fillLevel - 1; i > 0; i--)
            empty[i] = empty[i - 1];
        empty[0] = emptySlots;
    }
        
    speedCount += 1;
    if (speedCount >= 6) //o data la 5 generari mareste viteza
    {
        interval -= 100;
        speedCount = 0;
    }
    generatorStart = millis();
    generatorEnd = generatorStart + interval;
}

void showScore(bool part, int digit) //functie care afiseaza o cifra a scorului pe una din jumatatile matricei
{
    int leftColumn, rightColumn;
    if (part == 0)
    {
        leftColumn = 1;
        rightColumn = 3;
    }
    else
    {
        leftColumn = 5;
        rightColumn = 7;
    }
    switch (digit)
    {
        case 0:
            for (int i = 1; i < 6; i++)
                matrix[i][rightColumn] = 1;
            for (int i = 1; i < 6; i++)
                matrix[i][leftColumn] = 1;
            matrix[1][leftColumn + 1] = 1;
            matrix[5][leftColumn + 1] = 1;
        case 1:
            for (int i = 1; i < 6; i++)
                matrix[i][rightColumn] = 1;
            break;
        case 2:
            for (int j = leftColumn; j <= rightColumn; j++)
            {
                matrix[1][j] = 1;
                matrix[3][j] = 1;
                matrix[5][j] = 1;
            }
            matrix[2][rightColumn] = 1;
            matrix[4][leftColumn] = 1;
            break;
        case 3:
            for (int j = leftColumn; j <= rightColumn; j++)
            {
                matrix[1][j] = 1;
                matrix[3][j] = 1;
                matrix[5][j] = 1;
            }
            matrix[2][rightColumn] = 1;
            matrix[4][rightColumn] = 1;
            break;
        case 4:
            for (int i = 1; i < 6; i++)
                matrix[i][rightColumn] = 1;
            for (int i = 1; i < 4; i++)
                matrix[i][leftColumn] = 1;
            matrix[3][leftColumn + 1] = 1;
            break;
        case 5:
            for (int j = leftColumn; j <= rightColumn; j++)
            {
                matrix[1][j] = 1;
                matrix[3][j] = 1;
                matrix[5][j] = 1;
            }
            matrix[2][leftColumn] = 1;
            matrix[4][rightColumn] = 1;
            break;
        case 6:
            for (int j = leftColumn; j <= rightColumn; j++)
            {
                matrix[1][j] = 1;
                matrix[3][j] = 1;
                matrix[5][j] = 1;
            }
            matrix[2][leftColumn] = 1;
            matrix[4][leftColumn] = 1;
            matrix[4][rightColumn] = 1;
            break;
        case 7:
            for (int i = 1; i < 6; i++)
                matrix[i][rightColumn] = 1;
            matrix[1][leftColumn] = 1;
            matrix[1][leftColumn + 1] = 1;
            break;
        case 8:
            for (int i = 1; i < 6; i++)
                matrix[i][rightColumn] = 1;
            for (int i = 1; i < 6; i++)
                matrix[i][leftColumn] = 1;
            matrix[1][leftColumn + 1] = 1;
            matrix[3][leftColumn + 1] = 1;
            matrix[5][leftColumn + 1] = 1;
        case 9:
            for (int j = leftColumn; j <= rightColumn; j++)
            {
                matrix[1][j] = 1;
                matrix[3][j] = 1;
                matrix[5][j] = 1;
            }
            matrix[2][leftColumn] = 1;
            matrix[2][rightColumn] = 1;
            matrix[4][rightColumn] = 1;
    }
    for (int i = 0; i < 8; i++)
        for (int j = leftColumn; j <= rightColumn; j++)
            lc.setLed(0, i, j, matrix[i][j]);
}

void resetVariables()
{
    initial = 0; //se reseteaza statutul si variabilele jocului
    motionStart = millis();
    motionEnd = motionStart + 180;
    shipLeft = 2;
    shipRight = 4;
    shipCannon = (shipLeft + shipRight) / 2;
    fillLevel = 0;
    scoreCounter = 0;
    int resetMatrix[8][8]={ {0,0,0,1,1,0,0,0},
                            {0,0,1,0,0,1,0,0},
                            {0,1,0,0,0,0,1,0},
                            {1,0,0,1,1,0,0,1},
                            {1,0,0,1,1,0,0,1},
                            {0,1,0,0,0,0,1,0},
                            {0,0,1,0,0,1,0,0},
                            {0,0,0,1,1,0,0,0},
                          };
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            matrix[i][j] = resetMatrix[i][j];
    speedCount = 0;
    interval = 4000;
    generatorStart = millis();
    generatorEnd = generatorStart + interval;
    generatorPresent = 0;
    difficultyTimer = 0;
    difficultyStart = millis();
    difficultyEnd = difficultyStart + 350;
}

void setup()
{
    // zero se refera la chip
    lc.shutdown(0, false); // porneste display-ul, opreste modul de salvare baterie
    lc.setIntensity(0, 4); // seteaza luminozitatea
    lc.clearDisplay(0);// goleste display-ul

    pinMode(SW_PIN, INPUT);
    digitalWrite(SW_PIN, HIGH);

    randomSeed(analogRead(0)); //seed pentru generare random
}

void loop()
{
    if (initial == 0) //urmatoarea zona reprezinta meniul
    {

        startAnimation();
      
        matrix[1][2] = 1; //de aici
        matrix[1][3] = 1;
        matrix[1][4] = 1;
        matrix[1][5] = 1;
        matrix[2][2] = 1;
        matrix[3][2] = 1;
        matrix[3][3] = 1; //configuratia de puncte pestru "S" de la Start
        matrix[3][4] = 1;
        matrix[3][5] = 1;
        matrix[4][5] = 1;
        matrix[5][5] = 1;
        matrix[5][2] = 1;
        matrix[5][3] = 1;
        matrix[5][4] = 1; //pana aici

        matrix[7][2] = 1; //bara de dificultate
        matrix[7][3] = 1;
        matrix[7][4] = 1;
        difficultyValue = 4;

        for(int i = 0; i < 8; i++) //configuratia de leduri pentru animatia din ecranul de start
            if((i != 3) && (i != 7))
            {
                matrix[i][0] = 1;
                matrix[i][7] = 1;
            }

        for (int row = 0; row < 8; row++)
            for (int col = 0; col < 8; col++)
                lc.setLed(0, row, col, matrix[row][col]);

        unsigned long present; //timere pentru animatia de meniu
        unsigned long startTimer = millis();
        unsigned long endTimer = startTimer + 150;
        

        switchValue = digitalRead(SW_PIN);
        while (switchValue == 1) //cat timp jucatorul nu a apasat butonul de start
        {
            present = millis();
            if (present > endTimer)
            {
                for(int i = 0; i < 7; i++) //animatia de pe marginile ecranului
                {
                    matrix[i][0] = matrix[i + 1][0];
                    matrix[i][7] = matrix[i + 1][7];
                }
                if (matrix[3][0] == 0)
                {
                    matrix[7][0] = 0;
                    matrix[7][7] = 0;
                }
                else
                {
                    matrix[7][0] = 1;
                    matrix[7][7] = 1;
                }

                for(int i = 0; i <= 7; i++)
                {
                    lc.setLed(0, i, 0, matrix[i][0]);
                    lc.setLed(0, i, 7, matrix[i][7]);
                }
                startTimer = millis();
                endTimer = startTimer + 150;
            }

            xValue = analogRead(X_PIN);
            difficultyTimer = millis();
            if (xValue >= 875)
                increaseDifficulty();
            else
            if (xValue <= 150)
                decreaseDifficulty();
            switchValue = digitalRead(SW_PIN);
        }
          
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                matrix[i][j] = 0; //se goleste matricea pt a se pregati de joc
        
        matrix[7][2] = 1; //este creat jucatorul
        matrix[7][3] = 1;
        matrix[7][4] = 1;
        matrix[6][3] = 1;
          
        initial = 1; //s-a terminat configurarea
    }
    
    //aici s-a terminat meniul
    //de aici jocul in sine

    for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                lc.setLed(0, i, j, matrix[i][j]); //in fiecare loop se face o afisare completa a matricei
                
    xValue = analogRead(X_PIN);
    motionPresent = millis();
    if (motionPresent >= motionEnd) //in functie de timer, se decide daca jucatorul poate face o miscare
    {
        if (xValue <= 150)
            moveLeft();
        else
        if (xValue >= 875)
            moveRight();
        motionStart = millis();
        motionEnd = motionStart + 180;
    }

    generatorPresent = millis();
    if (generatorPresent >= generatorEnd) //in functie de timer, se decide daca se genereaza un rand nou
    {
        fillLevel += 1;
        if (fillLevel > 6) //daca jucatorul a pierdut
        {
            destroyed:
            for (int i = 0; i < 5; i++) //se reda de 5 ori animatia pentru explozie
                explosionAnimation();
                
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    matrix[i][j] = 0;
            for (int i = 0; i < 8; i++)
                    for (int j = 0; j < 8; j++)
                        lc.setLed(0, i, j, matrix[i][j]); //se goleste matricea
            delay(1500);
            if (scoreCounter > 99) //se afiseaza scorul pe cazuri
            {
                matrix[1][1] = 1;
                matrix[2][2] = 1;
                matrix[3][1] = 1;
                matrix[4][3] = 1;
                matrix[4][4] = 1;
                matrix[4][6] = 1;
                matrix[4][7] = 1;
                matrix[5][3] = 1;
                matrix[5][4] = 1;
                matrix[5][6] = 1;
                matrix[5][7] = 1;
                matrix[6][4] = 1;
                matrix[6][7] = 1;
                matrix[7][3] = 1;
                matrix[7][4] = 1;
                matrix[7][6] = 1;
                matrix[7][7] = 1;
                for (int i = 0; i < 8; i++)
                    for (int j = 0; j < 8; j++)
                        lc.setLed(0, i, j, matrix[i][j]);
            }
            else
            {
                showScore(1,scoreCounter % 10);
                scoreCounter = scoreCounter / 10;
                showScore(0,scoreCounter);
            }
            delay(5000); //timp de 5 secunde se afiseaza scorul
            for (int i = 0; i < 8; i++)
                for (int j = 0; j < 8; j++)
                    matrix[i][j] = 0;
            for (int i = 0; i < 8; i++)
                    for (int j = 0; j < 8; j++)
                        lc.setLed(0, i, j, matrix[i][j]);

            resetVariables();
            
            delay(1500);
            goto finish; //se restarteaza jocul
        }
        
        generateShape();
    }

    switchValue = digitalRead(SW_PIN);
    if (switchValue == 0) //jucatorul trage cu o bila
    {
        int row = 5;
        int col = shipCannon;
        if (matrix[row][col] == 1) //daca jucatorul trage dar nu are spatiu suficient explodeaza
            goto destroyed;
        matrix[row][col] = 1;
        lc.setLed(0, row, col, matrix[row][col]);
        delay(70);
        while (row - 1 >= 0 && matrix[row - 1][col] == 0) //bila parcurge matricea in sus pe cat posibil
        {
            matrix[row][col] = 0;
            lc.setLed(0, row, col, matrix[row][col]);
            row -= 1;
            matrix[row][col] = 1;
            lc.setLed(0, row, col, matrix[row][col]);
            delay(70);
        }
        if (row == fillLevel) //daca bila s-a pus pe un rand nou
        {
            fillLevel += 1;
            empty[row] = 5;
            matrix[row][0] = 1;
            matrix[row][7] = 1;
            lc.setLed(0, row, 0, matrix[row][0]);
            lc.setLed(0, row, 7, matrix[row][7]);
        }
        else //daca bila a umplut un gol
            empty[row] -= 1;
        if (empty[row] == 0) //se verifica daca randul pe care s-a pus bila s-a umplut
        {
            for (int i = row; i < 7; i++)
                empty[i] = empty[i + 1];
            if (row < fillLevel - 1) //daca da, se copiaza restul randurilor
            {
                for (int i = row; i < fillLevel - 1; i++)
                    for (int j = 0; j < 8; j++)
                        matrix[i][j] = matrix[i + 1][j];
            }
            for (int j = 0; j < 8; j++)
                matrix[fillLevel - 1][j] = 0; //se sterge
            for (int j = 0; j < 8; j++) //animatia de eliminare a randului
            {
                lc.setLed(0, row, j, 0);
                delay(100);
            }
            scoreCounter += 1;
            fillLevel -= 1;
        }
    }
    finish:
    int nothing = 0; //fara aceasta instructiune, exista eroare; nu se poate face trimitere la o eticheta goala
}
