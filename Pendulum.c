/* Change to test repo */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LENGTH 200
#define TIME_STEP 0.1

float Pendulum(float tau, float m, float l, float k, float T){
    
    /* Theta and dtheta/dt are static as they need memory */
    static float theta = 0;
    static float dtheta_dt = 0;

    /* Gravity */
    float g = 9.81;

    /* dtheta^2/dt^2 */
    float ddtheta_ddt;

    /* Theta in deg */
    float theta_deg;

    /* Solve for second derivative dtheta^2/dt^2 */
    ddtheta_ddt = (tau - m*g*l*sin(theta) - k*dtheta_dt)/(m*l*l);

    /* Integrate second derivative to find dtheta/dt(n+1), using forward Euler */
    dtheta_dt += ddtheta_ddt*T;

    /* Integrate first derivative to find theta(n+1), using forward Euler */
    theta += dtheta_dt*T;

    /* Convert to deg */
    theta_deg = theta*180/M_PI;

    return theta_deg;
}

int main()
{
    /* Define variables  */
    float t = 0;
    int i = 0;
    float tau = 0;
    float theta = 0;
    FILE *file_in;
    FILE *file_out;

    /* Open file generated from Xcos */
    file_in = fopen("tau_input.txt", "r");
    /* Open file for logger */
    file_out = fopen("theta_output.txt", "w");

    /* Implement iteration using while loop */
    while(i < LENGTH)
    {
        /* Read row from input file */
        if (fscanf(file_in, "%f", &tau) < 1)
        {
            printf("End of file");
            exit(0);
        }

        /* Run one step of the model */
        theta = Pendulum(tau, 0.5, 1, 0.5, TIME_STEP);

        /* Increment time and counter */
        t = t + TIME_STEP;
        i = i + 1;

        /* Log variables in the text file */
        fprintf(file_out,"%f %f %f\n", t, theta, tau);

    }

    #if MODE == SIGNAL
        fclose(file_in);
    #endif
    fclose(file_out);
    exit(0);
}
