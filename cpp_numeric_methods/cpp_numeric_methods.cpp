#include <iostream>

double func(double x, double a, double f)
{
	return a*pow(x,2)+f;
}


double func_primary(double x, double a, double f)
{
	return (a*pow(x,3)/3)+f*x;
}


double func_diff(double x, double a, double f)
{
	return 2*a*x;
}

double integral_analytical(double x1, double x2, double a, double f)
{
	return func_primary(x2, a, f) - func_primary(x1, a, f);
}

double integral_numeric_rect(double x1, double x2, int prec, double a, double f)
{
	double d = x2 - x1;
	double st = (double)d / (double)prec;
	double s = 0;
	for (int i = 0; i < prec; i++)
	{
		double cx = x1 + i * st;
		double cy = func(cx, a, f);
		s += st * cy;
	}
	return s;
}

double integral_numeric_trap(double x1, double x2, int prec, double a, double f)
{
	double d = x2 - x1;
	double st = (double)d / (double)prec;
	double s = 0;
	for (int i = 0; i < prec; i++)
	{
		double cx = x1 + i * st;
		double cy = func(cx, a, f);
		double cy_n = func(cx+st, a, f);
		double delta = cy_n - cy; 
		double sb = (st * delta) / 2;
		s += (st * cy) + sb;
	}	
	return s;	
}

double diff_numeric(double x, double dx, double a, double f)
{
	double y = func(x, a, f);
	double y2 = func(x + dx, a, f);
	double dy = y2 - y;
	return dy / dx;
}


int main()
{
    // 1 - Numeric integration
	double a = 10;
	double f = 5;
	double x1 = 0;
	double x2 = 10;

	int n = 10;
	int ninc = 10;

	double base_val = integral_analytical(x1, x2, a, f);

	while (n < 1000000)
	{
		double num_val_r = integral_numeric_rect(x1,x2,n,a,f);
		double error_r = num_val_r - base_val;
		double rel_error_r = (error_r / base_val) * 100;

		double num_val_t = integral_numeric_trap(x1, x2, n, a, f);
		double error_t = num_val_t - base_val;
		double rel_error_t = (error_t / base_val) * 100;

		std::cout << "n=" << n << "; ia = " << base_val << "; RECT: in = " << 
			num_val_r <<";  err = " << error_r << "; err = " << rel_error_r << "%" 
			<< "; TRAP: in = " << num_val_t <<	";  err = " << error_t << 
			"; err_r = " << rel_error_t << "%" << std::endl;
		n += ninc;
		ninc *= 10;
	}

	system("pause");

	//2 - numeric differentiation

	double x = 10.1;
	double dx = 10;
	base_val = func_diff(x, a, f);
	double error = 100000000000;
	double prev_error = 100000000000000;
	while (dx>0.000000001)
	{
		double num_val = diff_numeric(x, dx, a, f);
		prev_error = error;
		error = num_val - base_val;
		double rel_error = (error / base_val) * 100;
		if (error > prev_error) break;
		std::cout << "dx=" << dx << "; da = " << base_val << "; RECT: dn = " <<
			num_val << ";  err = " << error << "; err = " << rel_error << "%" <<
			std::endl;
		dx = dx / 10;
	}

	system("pause");

	//3 - numeric extremes
	x1 = -40;
	x2 = 40;
	// find bend
	double step = 6;
	double cx = x1;
	double nx = x1;
	double cy = 0;
	double ny = 0;
	int i = 0;
	bool down = false;
	bool ndown = false;
	bool hasbend = false;
	while (cx < x2)
	{
		cx = x1 + step * i;
		nx = cx + step;
		cy = func(cx, a, f);
		ny = func(nx, a, f);
		printf("%f; %f -> %f; %f\n", cx, cy, nx, ny);
		if (i == 0)
		{
			if (ny < cy) { down = true; }
			else { down = false; }
		}
		else
		{
			if (ny < cy) { ndown = true; }
			else { ndown = false; }

			if (ndown != down)
			{
				hasbend = true;
				cx -= step;
				break;
			}
		}
		i++;
	}

	if (hasbend)
	{
		double stx = cx;
		double sty = cy;
		double enx = nx;
		double eny = ny;

		double step = (enx - stx) / 10000;
		
		double xpos = stx + step;		

		double minx = stx;
		double miny = func(stx, a, f);

		if (down)
		{
			while (xpos < enx)
			{
				double ypos = func(xpos, a, f);
				if (miny > ypos) { miny = ypos; minx = xpos; }
				printf("%f; %f; extr %f; %f\n", xpos, ypos, minx, miny);
				xpos += step;
			}
		}
		else
		{
			while (xpos < enx)
			{
				double ypos = func(stx, a, f);
				if (miny < ypos) { miny = ypos; minx = xpos; }
				xpos += step;
			}
		}

		std::cout << "Extr point = (" << minx << "; " << miny << ")\n";

	}
	else
	{
		std::cout << "No bend fond, no extremes" << std::endl;
	}

	system("pause");

}