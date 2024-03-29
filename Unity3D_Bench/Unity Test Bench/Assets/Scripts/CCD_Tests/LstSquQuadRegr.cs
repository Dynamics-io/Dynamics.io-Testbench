﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace QuadRegressionTesting
{
    public class LstSquQuadRegr
    {
        /* instance variables */
        ArrayList pointArray = new ArrayList();
        private int numOfEntries;
        private double[] pointpair;

        public struct Variables
        {
            public double s40; //sum of x^4
            public double s30; //sum of x^3
            public double s20; //sum of x^2
            public double s10;  //sum of x

            public double s21; //sum of x^2*y
            public double s11;  //sum of x*y
            public double s01;   //sum of y
        }



        bool do_pre = false;

        /*constructor */
        public LstSquQuadRegr()
        {
            numOfEntries = 0;
            pointpair = new double[2];
        }

        /*instance methods */
        /// <summary>
        /// add point pairs
        /// </summary>
        /// <param name="x">x value</param>
        /// <param name="y">y value</param>
        public void AddPoints(double x, double y)
        {
            pointpair = new double[2];
            numOfEntries += 1;
            pointpair[0] = x;
            pointpair[1] = y;
            pointArray.Add(pointpair);
        }

        public Variables PreCalculate()
        {
            Variables v = new Variables();

            v.s40 = getSx4(); //sum of x^4
            v.s30 = getSx3(); //sum of x^3
            v.s20 = getSx2(); //sum of x^2
            v.s10 = getSx();  //sum of x

            v.s21 = getSx2y(); //sum of x^2*y
            v.s11 = getSxy();  //sum of x*y
            v.s01 = getSy();   //sum of y

            do_pre = true;

            return v;
        }

        /// <summary>
        /// returns the a term of the equation ax^2 + bx + c
        /// </summary>
        /// <returns>a term</returns>
        public double aTerm(Variables v = default(Variables))
        {
            if (numOfEntries < 3)
            {
                throw new InvalidOperationException(
                   "Insufficient pairs of co-ordinates");
            }

            double s40; //sum of x^4
            double s30; //sum of x^3
            double s20; //sum of x^2
            double s10;  //sum of x

            double s21; //sum of x^2*y
            double s11;  //sum of x*y
            double s01;   //sum of y

            if (!do_pre)
            {
                //notation sjk to mean the sum of x_i^j*y_i^k. 
                s40 = getSx4(); //sum of x^4
                s30 = getSx3(); //sum of x^3
                s20 = getSx2(); //sum of x^2
                s10 = getSx();  //sum of x

                //sum of x^0 * y^0  ie 1 * number of entries

                s21 = getSx2y(); //sum of x^2*y
                s11 = getSxy();  //sum of x*y
                s01 = getSy();   //sum of y
            }
            else
            {
                //notation sjk to mean the sum of x_i^j*y_i^k. 
                s40 = v.s40; //sum of x^4
                s30 = v.s30; //sum of x^3
                s20 = v.s20; //sum of x^2
                s10 = v.s10;  //sum of x

                //sum of x^0 * y^0  ie 1 * number of entries

                s21 = v.s21; //sum of x^2*y
                s11 = v.s11;  //sum of x*y
                s01 = v.s01;   //sum of y
            }

            double s00 = numOfEntries;

            //a = Da/D
            return (s21 * (s20 * s00 - s10 * s10) -
                    s11 * (s30 * s00 - s10 * s20) +
                    s01 * (s30 * s10 - s20 * s20))
                    /
                    (s40 * (s20 * s00 - s10 * s10) -
                     s30 * (s30 * s00 - s10 * s20) +
                     s20 * (s30 * s10 - s20 * s20));
        }

        /// <summary>
        /// returns the b term of the equation ax^2 + bx + c
        /// </summary>
        /// <returns>b term</returns>
        public double bTerm(Variables v = default(Variables))
        {
            if (numOfEntries < 3)
            {
                throw new InvalidOperationException(
                   "Insufficient pairs of co-ordinates");
            }

            double s40; //sum of x^4
            double s30; //sum of x^3
            double s20; //sum of x^2
            double s10;  //sum of x

            double s21; //sum of x^2*y
            double s11;  //sum of x*y
            double s01;   //sum of y

            if (!do_pre)
            {
                //notation sjk to mean the sum of x_i^j*y_i^k.
                s40 = getSx4(); //sum of x^4
                s30 = getSx3(); //sum of x^3
                s20 = getSx2(); //sum of x^2
                s10 = getSx();  //sum of x

                //sum of x^0 * y^0  ie 1 * number of entries

                s21 = getSx2y(); //sum of x^2*y
                s11 = getSxy();  //sum of x*y
                s01 = getSy();   //sum of y
            }
            else
            {
                //notation sjk to mean the sum of x_i^j*y_i^k. 
                s40 = v.s40; //sum of x^4
                s30 = v.s30; //sum of x^3
                s20 = v.s20; //sum of x^2
                s10 = v.s10;  //sum of x

                //sum of x^0 * y^0  ie 1 * number of entries

                s21 = v.s21; //sum of x^2*y
                s11 = v.s11;  //sum of x*y
                s01 = v.s01;   //sum of y
            }

            double s00 = numOfEntries;

            //b = Db/D
            return (s40 * (s11 * s00 - s01 * s10) -
                    s30 * (s21 * s00 - s01 * s20) +
                    s20 * (s21 * s10 - s11 * s20))
                    /
                    (s40 * (s20 * s00 - s10 * s10) -
                     s30 * (s30 * s00 - s10 * s20) +
                     s20 * (s30 * s10 - s20 * s20));
        }

        /// <summary>
        /// returns the c term of the equation ax^2 + bx + c
        /// </summary>
        /// <returns>c term</returns>
        public double cTerm(Variables v = default(Variables))
        {
            if (numOfEntries < 3)
            {
                throw new InvalidOperationException(
                           "Insufficient pairs of co-ordinates");
            }

            double s40; //sum of x^4
            double s30; //sum of x^3
            double s20; //sum of x^2
            double s10;  //sum of x

            double s21; //sum of x^2*y
            double s11;  //sum of x*y
            double s01;   //sum of y

            if (!do_pre)
            {
                //notation sjk to mean the sum of x_i^j*y_i^k.
                s40 = getSx4(); //sum of x^4
                s30 = getSx3(); //sum of x^3
                s20 = getSx2(); //sum of x^2
                s10 = getSx();  //sum of x

                //sum of x^0 * y^0  ie 1 * number of entries

                s21 = getSx2y(); //sum of x^2*y
                s11 = getSxy();  //sum of x*y
                s01 = getSy();   //sum of y
            }
            else
            {
                //notation sjk to mean the sum of x_i^j*y_i^k. 
                s40 = v.s40; //sum of x^4
                s30 = v.s30; //sum of x^3
                s20 = v.s20; //sum of x^2
                s10 = v.s10;  //sum of x

                //sum of x^0 * y^0  ie 1 * number of entries

                s21 = v.s21; //sum of x^2*y
                s11 = v.s11;  //sum of x*y
                s01 = v.s01;   //sum of y
            }

            double s00 = numOfEntries;

            //c = Dc/D
            return (s40 * (s20 * s01 - s10 * s11) -
                    s30 * (s30 * s01 - s10 * s21) +
                    s20 * (s30 * s11 - s20 * s21))
                    /
                    (s40 * (s20 * s00 - s10 * s10) -
                     s30 * (s30 * s00 - s10 * s20) +
                     s20 * (s30 * s10 - s20 * s20));
        }

        public double rSquare() // get r-squared
        {
            if (numOfEntries < 3)
            {
                throw new InvalidOperationException(
                   "Insufficient pairs of co-ordinates");
            }
            // 1 - (residual sum of squares / total sum of squares)
            return 1 - getSSerr() / getSStot();
        }


        /*helper methods*/
        private double getSx() // get sum of x
        {
            double Sx = 0;
            foreach (double[] ppair in pointArray)
            {
                Sx += ppair[0];
            }
            return Sx;
        }

        private double getSy() // get sum of y
        {
            double Sy = 0;
            foreach (double[] ppair in pointArray)
            {
                Sy += ppair[1];
            }
            return Sy;
        }

        private double getSx2() // get sum of x^2
        {
            double Sx2 = 0;
            foreach (double[] ppair in pointArray)
            {
                Sx2 += Math.Pow(ppair[0], 2); // sum of x^2
            }
            return Sx2;
        }

        private double getSx3() // get sum of x^3
        {
            double Sx3 = 0;
            foreach (double[] ppair in pointArray)
            {
                Sx3 += Math.Pow(ppair[0], 3); // sum of x^3
            }
            return Sx3;
        }

        private double getSx4() // get sum of x^4
        {
            double Sx4 = 0;
            foreach (double[] ppair in pointArray)
            {
                Sx4 += Math.Pow(ppair[0], 4); // sum of x^4
            }
            return Sx4;
        }

        private double getSxy() // get sum of x*y
        {
            double Sxy = 0;
            foreach (double[] ppair in pointArray)
            {
                Sxy += ppair[0] * ppair[1]; // sum of x*y
            }
            return Sxy;
        }

        private double getSx2y() // get sum of x^2*y
        {
            double Sx2y = 0;
            foreach (double[] ppair in pointArray)
            {
                Sx2y += Math.Pow(ppair[0], 2) * ppair[1]; // sum of x^2*y
            }
            return Sx2y;
        }

        private double getYMean() // mean value of y
        {
            double y_tot = 0;
            foreach (double[] ppair in pointArray)
            {
                y_tot += ppair[1];
            }
            return y_tot / numOfEntries;
        }

        private double getSStot() // total sum of squares
        {
            //the sum of the squares of the differences between 
            //the measured y values and the mean y value
            double ss_tot = 0;
            foreach (double[] ppair in pointArray)
            {
                ss_tot += Math.Pow(ppair[1] - getYMean(), 2);
            }
            return ss_tot;
        }

        private double getSSerr() // residual sum of squares
        {
            //the sum of the squares of te difference between 
            //the measured y values and the values of y predicted by the equation
            double ss_err = 0;
            foreach (double[] ppair in pointArray)
            {
                ss_err += Math.Pow(ppair[1] - getPredictedY(ppair[0]), 2);
            }
            return ss_err;
        }

        private double getPredictedY(double x)
        {
            //returns value of y predicted by the equation for a given value of x
            return aTerm() * Math.Pow(x, 2) + bTerm() * x + cTerm();
        }
    }
}
