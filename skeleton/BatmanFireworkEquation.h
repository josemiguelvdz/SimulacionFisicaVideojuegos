#pragma once

#include <math.h>
#include <stdlib.h>


class BatmanFireWorkEquation {
public:
    BatmanFireWorkEquation() {
        // Ancara mesi
    };

    void DoEquation() {
        double val1 = 0.0, val2 = 0.0, x = 0.0;


        for (x = -7; x <= 7; x += 0.01)
        {
            if (x != 0)
            {
                if ((x > -1 || x < -3) && (x < 1) || x > 3)
                {

                    val1 = 2 * sqrt((-fabs(fabs(x) - 1)) * fabs(3 - fabs(x)) / ((fabs(x) - 1) * (3 - fabs(x)))) * (1 + fabs(fabs(x) - 3) / (fabs(x) - 3)) * sqrt(1 - (x / 7) * (x / 7)) + (5 + 0.97 * (fabs(x - 0.5) + fabs(x + 0.5)) - 3 * (fabs(x - 0.75) + fabs(x + 0.75))) * (1 + fabs(1 - fabs(x)) / (1 - fabs(x)));
                }
                if ((x > -3 || x < -7) && (x > 1 || x < -1) && (x < 3 || x>7))
                {
                    val1 = (2.71052 + 1.5 - 0.5 * fabs(x) - 1.35526 * sqrt(4 - (fabs(x) - 1) * (fabs(x) - 1))) * sqrt(fabs(fabs(x) - 1) / (fabs(x) - 1)) + 0.9;
                }

                // fprintf(fp1, "%lf   %lf\n", x, val1);

            }

            {if (x > 4 || x < -4) { val2 = (-3) * sqrt(1 - (x / 7) * (x / 7)) * sqrt(fabs(fabs(x) - 4) / (fabs(x) - 4)); }


            if ((x > -4 && x < 4)) {
                val2 = fabs(x / 2) - 0.0913722 * x * x - 3 + sqrt(1 - (fabs(fabs(x) - 2) - 1) * (fabs(fabs(x) - 2) - 1));
            }

                // fprintf(fp2, "%lf   %lf\n", x, val2);
            }

        }
    }
};
