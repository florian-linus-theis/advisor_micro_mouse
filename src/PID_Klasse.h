#include <vector>
#include <iostream>
#include <stdlib.h>
using namespace std;

vector<double> SensorInput;

class PID
{
    private:
        vector<double> errorVec{0,0,0,0,0,0}, tempOut{0,0,0,0,0,0}, previousErrorVec{0,0,0}, errorDivideVec{0,0,0}, OutputWeight{1,1,1};
        double kp, ki, kd, dt;
        double last_time=0;
        double integral, output_G = 0;
        double setpoint = 255;
        double accepted_error;
        double rotation_output, x_output, y_output;
        double out_L, out_R;
        double rotation_error, x_error, y_error;

        //Wertebereiche Sensor-Input.
        //Zuordnung: Outer_Left, Inner_Left, Front_Left, Front_Right, Inner_Right, Outer_Right
        vector<int> upperBoundarys{0,0,0,0,0,0};
        vector<int> lowerBoundarys{1024,1024,1024,1024,1024,1024};
        vector<double> NeutralSensorValues{4.12 , 4.47 , 5.27 , 5.27 , 4.47 , 4.12 };

    public:
        PID(double kp=0.8, double ki=1.0, double kd=0.0001, double dt=1.0){
            this->kp = kp;
            this->ki = ki;
            this->kd = kd;
            this->dt = dt;
        }

        double applyPID(double previous, double error)
        {
            double proportional = error;
            integral += error * dt;
            double derivative = (error - previous) / dt;
            previous = error;
            double output = (kp * proportional) + (ki * integral) + (kd * derivative);
            return output;
        }

        vector<double> calcErrors(vector<double> SensorInput, vector<bool> walls){
            //error-Werte für jeden Sensor berechnen
            for(int i=0; i<6; i++){
                errorVec[i] = SensorInput[i]-NeutralSensorValues[i];
                cout << errorVec[i] << endl;
            }

            //walls[0] = Wand links, Wall[1] = Wand vorne, Walls[2] = Wand rechts
            //True = Wand erkannt, False = Keine Wand erkannt

            if(walls[0]==true){
                rotation_error = (errorVec[0]-errorVec[1]);
                x_error += errorVec[1] + errorVec[0];
                y_error += 0;

                errorDivideVec[0] += 2; //Anzahl der Sensoren, die Signal beitragen zählen --> Später zum Durchschnitt bilden benötigt
                errorDivideVec[1] += 2;
                errorDivideVec[2] += 0;
            }

            if(walls[1] == true){
                rotation_error += (errorVec[2]-errorVec[3]);
                x_error += 0;
                y_error += (errorVec[2]+errorVec[3]);

                errorDivideVec[0] += 2;
                errorDivideVec[1] += 0;
                errorDivideVec[2] += 2;
            }

            if(walls[2] == true){
                rotation_error += (errorVec[4]-errorVec[5]);
                x_error += -errorVec[4] - errorVec[5];
                y_error += 0;

                errorDivideVec[0] += 2;
                errorDivideVec[1] += 2;
                errorDivideVec[2] += 0;
            }
            if (errorDivideVec[0] == 0){
                rotation_error = 0;
            } else { 
                rotation_error = rotation_error/(errorDivideVec[0]);
            }
            if (errorDivideVec[1] == 0){
                x_error = 0; 
            } else {
                x_error = x_error/(errorDivideVec[1]);
            }
            if (errorDivideVec[2] == 0){
                y_error = 0;
            } else {
                y_error = y_error/(errorDivideVec[2]);
            }
            cout << rotation_error << " -- " << x_error << " -- " << y_error << endl;

            return {rotation_error,x_error,y_error};
        }


        vector<double> calcOutput(vector<double> SensorInput, vector<bool> walls){
            //Error-Werte berechnen lassen
            vector<double> ErrorInputVec = calcErrors(SensorInput, walls);

            //Checken ob ein Rotation_error mit den ggb. Daten ausrechenbar ist
            if(walls[0] == true || walls[1] == true || walls[2] == true){
                rotation_output = applyPID(previousErrorVec[0],ErrorInputVec[0]);
                previousErrorVec[0] = ErrorInputVec[0];
            }
            else{
                rotation_output = 0;
                previousErrorVec[0] = 0;
            }

            //Checken ob ein x_error mit den ggb. Daten ausrechenbar ist
            if(walls[0] == true || walls[2] == true){
                x_output = applyPID(previousErrorVec[1],ErrorInputVec[1]);
                previousErrorVec[1] = ErrorInputVec[1];
            }
            else{
                x_output = 0;
                previousErrorVec[1] = 0;
            }

            //Checken ob ein y_error mit den ggb. Daten ausrechenbar ist
            if(walls[1] == true){
                y_output = applyPID(previousErrorVec[2],ErrorInputVec[2]);
                previousErrorVec[2] = ErrorInputVec[2];
            }
            else{
                y_output = 0;
                previousErrorVec[2] = 0;
            }

            out_L = -OutputWeight[0]*rotation_output + OutputWeight[0]*x_output + OutputWeight[2]*y_output;
            out_R = OutputWeight[0]*rotation_output - OutputWeight[0]*x_output + OutputWeight[2]*y_output;

            return {out_L,out_R};
        }
};

