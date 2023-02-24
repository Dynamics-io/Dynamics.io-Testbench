struct Variables {
	float s40; //sum of x^4
	float s30; //sum of x^3
	float s20; //sum of x^2
	float s10;  //sum of x

	float s21; //sum of x^2*y
	float s11;  //sum of x*y
	float s01;   //sum of y
} Variables;

float getSx(float points[3][2]) // get sum of x
{
	float Sx = 0;
	for(int i = 0; i < 3; i++)
	{
		Sx += points[i][0];
	}
	return Sx;
}

float getSy(float points[3][2]) // get sum of y
{
	float Sy = 0;
	for(int i = 0; i < 3; i++)
	{
		Sy += points[i][1];
	}
	return Sy;
}

float getSx2(float points[3][2]) // get sum of x^2
{
	float Sx2 = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx2 += pow(points[i][0], 2); // sum of x^2
	}
	return Sx2;
}

float getSx3(float points[3][2]) // get sum of x^3
{
	float Sx3 = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx3 += pow(points[i][0], 3); // sum of x^3
	}
	return Sx3;
}

float getSx4(float points[3][2]) // get sum of x^4
{
	float Sx4 = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx4 += pow(points[i][0], 4); // sum of x^4
	}
	return Sx4;
}

float getSxy(float points[3][2]) // get sum of x*y
{
	float Sxy = 0;
	for (int i = 0; i < 3; i++)
	{
		Sxy += points[i][0] * points[i][1]; // sum of x*y
	}
	return Sxy;
}

float getSx2y(float points[3][2]) // get sum of x^2*y
{
	float Sx2y = 0;
	for (int i = 0; i < 3; i++)
	{
		Sx2y += pow(points[i][0], 2) * points[i][1]; // sum of x^2*y
	}
	return Sx2y;
}

struct Variables regress3_preCalculate(float points[3][2])
{
	struct Variables v;

	v.s40 = getSx4(points); //sum of x^4
	v.s30 = getSx3(points); //sum of x^3
	v.s20 = getSx2(points); //sum of x^2
	v.s10 = getSx(points);  //sum of x

	v.s21 = getSx2y(points); //sum of x^2*y
	v.s11 = getSxy(points);  //sum of x*y
	v.s01 = getSy(points);   //sum of y

	return v;
}

float regress3_aTerm(struct Variables v)
{
	//notation sjk to mean the sum of x_i^j*y_i^k. 
	float s40 = v.s40; //sum of x^4
	float s30 = v.s30; //sum of x^3
	float s20 = v.s20; //sum of x^2
	float s10 = v.s10;  //sum of x

	//sum of x^0 * y^0  ie 1 * number of entries

	float s21 = v.s21; //sum of x^2*y
	float s11 = v.s11;  //sum of x*y
	float s01 = v.s01;   //sum of y
	

	float s00 = 3;

	//a = Da/D
	return (s21 * (s20 * s00 - s10 * s10) -
		s11 * (s30 * s00 - s10 * s20) +
		s01 * (s30 * s10 - s20 * s20))
		/
		(s40 * (s20 * s00 - s10 * s10) -
			s30 * (s30 * s00 - s10 * s20) +
			s20 * (s30 * s10 - s20 * s20));
}

float regress3_bTerm(struct Variables v)
{
	//notation sjk to mean the sum of x_i^j*y_i^k. 
	float s40 = v.s40; //sum of x^4
	float s30 = v.s30; //sum of x^3
	float s20 = v.s20; //sum of x^2
	float s10 = v.s10;  //sum of x

	//sum of x^0 * y^0  ie 1 * number of entries

	float s21 = v.s21; //sum of x^2*y
	float s11 = v.s11;  //sum of x*y
	float s01 = v.s01;   //sum of y


	float s00 = 3;

	//b = Db/D
	return (s40 * (s11 * s00 - s01 * s10) -
		s30 * (s21 * s00 - s01 * s20) +
		s20 * (s21 * s10 - s11 * s20))
		/
		(s40 * (s20 * s00 - s10 * s10) -
			s30 * (s30 * s00 - s10 * s20) +
			s20 * (s30 * s10 - s20 * s20));
}

float regress3_cTerm(struct Variables v)
{
	//notation sjk to mean the sum of x_i^j*y_i^k. 
	float s40 = v.s40; //sum of x^4
	float s30 = v.s30; //sum of x^3
	float s20 = v.s20; //sum of x^2
	float s10 = v.s10;  //sum of x

	//sum of x^0 * y^0  ie 1 * number of entries

	float s21 = v.s21; //sum of x^2*y
	float s11 = v.s11;  //sum of x*y
	float s01 = v.s01;   //sum of y


	float s00 = 3;

	//c = Dc/D
	return (s40 * (s20 * s01 - s10 * s11) -
		s30 * (s30 * s01 - s10 * s21) +
		s20 * (s30 * s11 - s20 * s21))
		/
		(s40 * (s20 * s00 - s10 * s10) -
			s30 * (s30 * s00 - s10 * s20) +
			s20 * (s30 * s10 - s20 * s20));
}