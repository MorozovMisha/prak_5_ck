#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define N 1000
#define EPSILON 0.01 


void form_t(int n, float* t, float t_nac, float t_kon) {
    float dt = (t_kon - t_nac) / (n - 1); 
    for (int i = 0; i < n; i++) {
        t[i] = t_nac + i * dt;
    }
}


void form_Uvx(int n, float* t, float* Uvx, float a0, float a1, float a2) {
    for (int i = 0; i < n; i++) {
        Uvx[i] = a0 + a1 * t[i] + a2 * t[i] * t[i];
    }
}


void form_Uvix(int n, float* Uvx, float* Uvix, float a, float b, float Uvx1) {
    for (int i = 0; i < n; i++) {
        if (Uvx[i] < Uvx1) {
            Uvix[i] = a;
        } else {
            Uvix[i] = b * Uvx[i] * Uvx[i];
        }
    }
}


void draw_console_graph(int n, float* U, float Umax, float Umin,float t_nac,float t_kon) {
    int y_max = 100;
    float y_scale = (Umax - Umin) / y_max; 

    
    printf("\t------------------------------------------------------------------------------------------------------\n");

    for (int j = y_max; j >= 0; j--) {
        printf("%6.2f |", Umin + j * y_scale);

       
        for (int i = 0; i <= n; i++) {
            if (U[i] >= Umin + j * y_scale) {
                printf("**");
            } else {
                printf("  ");
            }
        }
        printf("|\n");
    }
    printf("\t------------------------------------------------------------------------------------------------------\n");

    printf("\t");
    float i = 1;
    while ( i <= t_kon ) {
        printf("%4.2f ",i);
        i+=0.45;
    }
    printf("\n");
}





float find_time_by_value(int n, float* U, float* t, float value) {
    for (int i = 0; i < n; i++) {
        if (U[i] > value) {
            return t[i];
        }
    }
    return -1; 
}

float calculate_time_parameter(float t_nac, float t_kon, float a0, float a1, float a2,float value, float parameter_function(int, float*, float*, float)) {
    int n = 11;
    float dt = (t_kon - t_nac) / (n - 1);
    float t[N], Uvx[N];
    float par, par1, p = 1;

    
    form_t(n, t, t_nac, t_kon);
    form_Uvx(n, t, Uvx, a0, a1, a2);
    par = parameter_function(n, Uvx, t, value);

    
    while (fabs(par - par1) / par1 > EPSILON) {
        n *= 2;
        dt = (t_kon - t_nac) / (n - 1);
        form_t(n, t, t_nac, t_kon);
        form_Uvx(n, t, Uvx, a0, a1, a2);
        par1 = parameter_function(n, Uvx, t, value);
        p = fabs(par - par1) / par1;
        par = par1;
        printf("n=%d, parametr=%.3f, pogreshonst=%.4f\n", n, par1, p);
    }

    return par;
}

void print_table(int n, float* t, float* Uvx, float* Uvix) {
    printf("\nNum\t t\t Uvx\t Uvix\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t %.2f\t %.2f\t %.2f\n", i, t[i], Uvx[i], Uvix[i]);
    }
}


void read_zastavka(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error open file %s!\n", filename);
        exit(1);
    }

    char ch;
    while ((ch = fgetc(file)) != EOF) {
        printf("%c", ch);
    }

    fclose(file);
}
void ab(){
       float t_nac = 1, t_kon = 10, a0 = 2, a1 = 5, a2 = 0.3;
    float time_80_tochnost;
                time_80_tochnost = calculate_time_parameter(t_nac, t_kon, a0, a1, a2, 80, find_time_by_value);
                printf("Moment vremeni, kogda Uvx dostigaet 80 V s tochnost'yu %.2f: %.3f\n", EPSILON, time_80_tochnost);
}
void waitForEnter() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    
    read_zastavka("C:\\Users\\tutr\\Documents\\kursach\\zast.txt");

    while (1) {
        char *xf1;
        float t_nac = 1, t_kon = 10, a0 = 2, a1 = 5, a2 = 0.3;
        float a = 5, b = 0.05, Uvx1 = 10;
        float t[N], Uvx[N], Uvix[N];
        int n=50;
        char ch,enter;

        printf("\nList of varinats:\n");
        printf("1 - raschet for n dotes\n");
        printf("2 - Nahojdenie momenta vremeni\n");
        
        printf("3 - Enter data to the file\n");
        printf("4 - Postroenie grafika\n");
        printf("5 - raschet parametra s zadannoi tochnosty\n");
        printf("6 - End\n");
        printf("Choose number: ");
        scanf(" %c", &ch);

        switch (ch) {
            case '1':
                printf("Enter number of dotes: ");
                scanf("%d", &n);
                form_t(n, t, t_nac, t_kon);
                form_Uvx(n, t, Uvx, a0, a1, a2);
                form_Uvix(n, Uvx, Uvix, a, b, Uvx1);
                print_table(n, t, Uvx, Uvix);
                break;
            case '2':
                form_t(n, t, t_nac, t_kon);
                form_Uvx(n, t, Uvx, a0, a1, a2);
                float time_80 = find_time_by_value(n, Uvx, t, 80);
                if (time_80 != -1) {
                    printf("Moment vremeni, kogda Uvx dostigaet 80 V: %.3f\n", time_80);
                } else {
                    printf("Uvx ne dostigaet 80 V.\n");
                }
                break;
            case '3':
                printf("Enter number of dotes to the file: ");
                scanf("%d", &n);
                form_t(n, t, t_nac, t_kon);
                form_Uvx(n, t, Uvx, a0, a1, a2);
                form_Uvix(n, Uvx, Uvix, a, b, Uvx1);

                FILE* file1 = fopen("t.txt", "w");
                FILE* file2 = fopen("Uvx.txt", "w");
                FILE* file3 = fopen("Uvix.txt", "w");
                if (file1 == NULL || file2 == NULL || file3 == NULL ) {
                    printf("Error open file!\n");
                    return 1; 
                }

                for (int i = 0; i < n; i++) {
                    fprintf(file1, "%.3f\n", t[i]);
                    fprintf(file2, "%.3f\n", Uvx[i]);
                    fprintf(file3, "%.3f\n",Uvix[i]);
                }

                fclose(file1);
                fclose(file2);
                fclose(file3);
                break;
            case '4':
                form_t(n, t, t_nac, t_kon);
                form_Uvx(n, t, Uvx, a0, a1, a2);
                form_Uvix(n, Uvx, Uvix, a, b, Uvx1);

                
                float Umax = Uvx[0], Umin = Uvx[0];
                for (int i = 1; i < n; i++) {
                    if (Uvx[i] > Umax) Umax = Uvx[i];
                    if (Uvx[i] < Umin) Umin = Uvx[i];
                }
                float Uvix_min = Uvix[0];
                float Uvix_max = Uvix[0];
                for (int i = 1; i < n; i++) {
                    if (Uvix[i] < Uvix_min) {
                        Uvix_min = Uvix[i];
                    }
                    if (Uvix[i] > Uvix_max) {
                        Uvix_max = Uvix[i];
                    }
                }
                

                printf("Graphic Uvx(t):\n");
                draw_console_graph(n, Uvx, Umax, Umin,t_nac,t_kon);
                printf("Graphic Uvix(t):\n");
                 draw_console_graph(n, Uvix, Uvix_max, Uvix_min, t_nac, t_kon); 
                break;
            case '5': 
                ab();
                break;
            case '6': 
                xf1="no";
                break;
            default:
                printf("Error enter.\n");
                break;
        }
        printf("Press enter to contiunue...");
         waitForEnter();
        system("cls");
        if (strcmp(xf1,"no")==0){
            return 0;
    }
    }

    return 0;
}