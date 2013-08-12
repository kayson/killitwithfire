#include "BlackBodyRadiation.h"
#include <stdio.h>
#include <stdlib.h>  
#include <cmath>
#include "Vector3.h"
#include "firePresets.h"

#include "GridField.hpp"
#include "LevelSet.h"

#include "SmokeDensity.h"

#include <omp.h>

const double PI = 3.14159265359;

const double C_1 = 3.7418e-16;
const double C_2 = 1.4388e-2;

// CIE starndard observer 1931
// http://www.cis.rit.edu/mcsl/online/cie.php
const double CIE_X[] = {1.299000e-04, 2.321000e-04, 4.149000e-04, 7.416000e-04, 1.368000e-03, 
2.236000e-03, 4.243000e-03, 7.650000e-03, 1.431000e-02, 2.319000e-02, 
4.351000e-02, 7.763000e-02, 1.343800e-01, 2.147700e-01, 2.839000e-01, 
3.285000e-01, 3.482800e-01, 3.480600e-01, 3.362000e-01, 3.187000e-01, 
2.908000e-01, 2.511000e-01, 1.953600e-01, 1.421000e-01, 9.564000e-02, 
5.795001e-02, 3.201000e-02, 1.470000e-02, 4.900000e-03, 2.400000e-03, 
9.300000e-03, 2.910000e-02, 6.327000e-02, 1.096000e-01, 1.655000e-01, 
2.257499e-01, 2.904000e-01, 3.597000e-01, 4.334499e-01, 5.120501e-01, 
5.945000e-01, 6.784000e-01, 7.621000e-01, 8.425000e-01, 9.163000e-01, 
9.786000e-01, 1.026300e+00, 1.056700e+00, 1.062200e+00, 1.045600e+00, 
1.002600e+00, 9.384000e-01, 8.544499e-01, 7.514000e-01, 6.424000e-01, 
5.419000e-01, 4.479000e-01, 3.608000e-01, 2.835000e-01, 2.187000e-01, 
1.649000e-01, 1.212000e-01, 8.740000e-02, 6.360000e-02, 4.677000e-02, 
3.290000e-02, 2.270000e-02, 1.584000e-02, 1.135916e-02, 8.110916e-03, 
5.790346e-03, 4.106457e-03, 2.899327e-03, 2.049190e-03, 1.439971e-03, 
9.999493e-04, 6.900786e-04, 4.760213e-04, 3.323011e-04, 2.348261e-04, 
1.661505e-04, 1.174130e-04, 8.307527e-05, 5.870652e-05, 4.150994e-05, 
2.935326e-05, 2.067383e-05, 1.455977e-05, 1.025398e-05, 7.221456e-06, 
5.085868e-06, 3.581652e-06, 2.522525e-06, 1.776509e-06, 1.251141e-06};

const double CIE_Y[] = {3.917000e-06, 6.965000e-06, 1.239000e-05, 2.202000e-05, 3.900000e-05, 
6.400000e-05, 1.200000e-04, 2.170000e-04, 3.960000e-04, 6.400000e-04, 
1.210000e-03, 2.180000e-03, 4.000000e-03, 7.300000e-03, 1.160000e-02, 
1.684000e-02, 2.300000e-02, 2.980000e-02, 3.800000e-02, 4.800000e-02, 
6.000000e-02, 7.390000e-02, 9.098000e-02, 1.126000e-01, 1.390200e-01, 
1.693000e-01, 2.080200e-01, 2.586000e-01, 3.230000e-01, 4.073000e-01, 
5.030000e-01, 6.082000e-01, 7.100000e-01, 7.932000e-01, 8.620000e-01, 
9.148501e-01, 9.540000e-01, 9.803000e-01, 9.949501e-01, 1.000000e+00, 
9.950000e-01, 9.786000e-01, 9.520000e-01, 9.154000e-01, 8.700000e-01, 
8.163000e-01, 7.570000e-01, 6.949000e-01, 6.310000e-01, 5.668000e-01, 
5.030000e-01, 4.412000e-01, 3.810000e-01, 3.210000e-01, 2.650000e-01, 
2.170000e-01, 1.750000e-01, 1.382000e-01, 1.070000e-01, 8.160000e-02, 
6.100000e-02, 4.458000e-02, 3.200000e-02, 2.320000e-02, 1.700000e-02, 
1.192000e-02, 8.210000e-03, 5.723000e-03, 4.102000e-03, 2.929000e-03, 
2.091000e-03, 1.484000e-03, 1.047000e-03, 7.400000e-04, 5.200000e-04, 
3.611000e-04, 2.492000e-04, 1.719000e-04, 1.200000e-04, 8.480000e-05, 
6.000000e-05, 4.240000e-05, 3.000000e-05, 2.120000e-05, 1.499000e-05, 
1.060000e-05, 7.465700e-06, 5.257800e-06, 3.702900e-06, 2.607800e-06, 
1.836600e-06, 1.293400e-06, 9.109300e-07, 6.415300e-07, 4.518100e-07};

const double CIE_Z[] = {6.061000e-04, 1.086000e-03, 1.946000e-03, 3.486000e-03, 6.450001e-03, 
1.054999e-02, 2.005001e-02, 3.621000e-02, 6.785001e-02, 1.102000e-01, 
2.074000e-01, 3.713000e-01, 6.456000e-01, 1.039050e+00, 1.385600e+00, 
1.622960e+00, 1.747060e+00, 1.782600e+00, 1.772110e+00, 1.744100e+00, 
1.669200e+00, 1.528100e+00, 1.287640e+00, 1.041900e+00, 8.129501e-01, 
6.162000e-01, 4.651800e-01, 3.533000e-01, 2.720000e-01, 2.123000e-01, 
1.582000e-01, 1.117000e-01, 7.824999e-02, 5.725001e-02, 4.216000e-02, 
2.984000e-02, 2.030000e-02, 1.340000e-02, 8.749999e-03, 5.749999e-03, 
3.900000e-03, 2.749999e-03, 2.100000e-03, 1.800000e-03, 1.650001e-03, 
1.400000e-03, 1.100000e-03, 1.000000e-03, 8.000000e-04, 6.000000e-04, 
3.400000e-04, 2.400000e-04, 1.900000e-04, 1.000000e-04, 4.999999e-05, 
3.000000e-05, 2.000000e-05, 1.000000e-05, 0.000000e+00, 0.000000e+00, 
0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 
0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 
0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 
0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 
0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 
0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 
0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00, 0.000000e+00};

BlackBodyRadiation::BlackBodyRadiation()
{
	Le = NULL;
	image = NULL;
	L = NULL;

	LeMean = NULL;
	xm = NULL;
	ym = NULL;
	zm = NULL;
}

BlackBodyRadiation::BlackBodyRadiation(const int XPIXELS, const int YPIXELS, const GridField<double> &temperatureGrid)
{
	xsize = XPIXELS;
	ysize = YPIXELS;
	zsize = temperatureGrid.zdim()*2.0;

	gridx = temperatureGrid.xdim();
	gridy = temperatureGrid.ydim();
	gridz = temperatureGrid.zdim();

	allocate();
}

BlackBodyRadiation::BlackBodyRadiation(const BlackBodyRadiation & other)
{
	xsize = other.xsize;
	ysize = other.ysize;
	zsize = other.zsize;

	gridx = other.gridx;
	gridy = other.gridy;
	gridz = other.gridz;

	allocate();
}

BlackBodyRadiation & BlackBodyRadiation::operator= (const BlackBodyRadiation & other)
{
	if(this != &other)
	{
		deallocate();

		xsize = other.xsize;
		ysize = other.ysize;
		zsize = other.zsize;

		gridx = other.gridx;
		gridy = other.gridy;
		gridz = other.gridz;

		allocate();
	}
	return *this;
}

void BlackBodyRadiation::deallocate()
{
	if(image != NULL) delete [] image;
	glDeleteTextures( 1, &textureID );

	if(Le != NULL) delete [] Le;
	if(L != NULL) delete [] L;

	if(LeMean != NULL) delete [] LeMean;
	if(xm != NULL) delete [] xm;
	if(ym != NULL) delete [] ym;
	if(zm != NULL) delete [] zm;
}

//Def xsize och gridx först osv.
void BlackBodyRadiation::allocate()
{
	dx = 1.0/double(xsize);
	dy = 1.0/double(ysize);
	dz = 1.0/double(zsize);

	du = 2.0/double(xsize-1);
	dv = 2.0/double(ysize-1);

	const int IMSIZE= xsize*ysize*3;
	image = new GLfloat[IMSIZE];
	#define GL_CLAMP_TO_EDGE 0x812F
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	wds = double(FirePresets::GRID_SIZE)/double(std::max(gridx, std::max(gridy, gridz)) * FirePresets::SAMPLE_STEP_QUALITY);

	oa = 0.05; //absorberings koef, för rapport 1 använd 0.01
	os = 0.0; //scattering koef
	ot = oa + os; //tot
	C = exp(-ot*wds);

	//Används för att rendera rummet, kan nog vara lägre samplad än total_samples, alt gör man denna med macgrid för slippa göra samma beräkningar 2 ggr
	//dock tror jag det kostar förmycket minne
	int LeSize = gridx*gridy*gridz*FirePresets::TOTAL_SAMPLES;
	Le = new double[LeSize]; 
	
	L = new double[omp_get_max_threads()*FirePresets::TOTAL_SAMPLES];

	LeMean = new double[FirePresets::TOTAL_SAMPLES];
	xm = new double[FirePresets::TOTAL_SAMPLES];
	ym = new double[FirePresets::TOTAL_SAMPLES];
	zm = new double[FirePresets::TOTAL_SAMPLES];

	angle = 0;

	funVar2 = 5.0;
}


BlackBodyRadiation::~BlackBodyRadiation()
{
	deallocate();
}

double BlackBodyRadiation::radiance(double lambda, double T)
{
	// Equation 22, Nguyen02
	return (2.0*C_1) / (pow(lambda,5.0) * (exp(C_2/(lambda*T)) - 1.0) );
}

Vector3 BlackBodyRadiation::blackbodyToXYZ(double T)
{
    
	Vector3 xyz(0.0, 0.0, 0.0);
	/*int step = 0;
	#pragma omp parallel for	
	for(int i = 360; i < 830; i+=5,step++) 
	{
		xyz.x += radiance(i,T) * CIE_X[step];
		xyz.y += radiance(i,T) * CIE_Y[step];
		xyz.z += radiance(i,T) * CIE_Z[step];
	}
	
	double totXYZ = xyz.x + xyz.y + xyz.z;
	xyz.x /= totXYZ;
	xyz.y /= totXYZ;
	xyz.z /= totXYZ;
     */
	return xyz;
}

Vector3 BlackBodyRadiation::XYZtoLMS(const Vector3 &xyz)
{
	Vector3 lms(0.0, 0.0, 0.0);

	// D65
	/*lms.x = xyz.x * 0.400	+ xyz.y * 0.708		+ xyz.z * -0.081;
	lms.y = xyz.x * -0.226	+ xyz.y * 1.165		+ xyz.z * 0.046;
	lms.z = xyz.x * 0		+ xyz.y * 0			+ xyz.z * 0.918;*/

	//CAT02
	lms.x =  0.7328*xyz.x + 0.4296*xyz.y - 0.1624*xyz.z;
	lms.y = -0.7036*xyz.x + 1.6975*xyz.y + 0.0061*xyz.z;
	lms.z =  0.0030*xyz.x + 0.0136*xyz.y + 0.9834*xyz.z;

	return lms;
}

Vector3 BlackBodyRadiation::LMStoXYZ(const Vector3 &lms)
{
	Vector3 xyz(0.0, 0.0, 0.0);

	// D65
	/*xyz.x = lms.x * 1.861	+ lms.y * -1.131	+ lms.z * 2.209;
	xyz.y = lms.x * 0.361	+ lms.y * 0.639		+ lms.z * -0.002;
	xyz.z = lms.x * 0		+ lms.y * 0			+ lms.z * 10.89;*/

	//CAT02
	xyz.x =  1.09612	*lms.x    - 0.278869*lms.y + 0.182745  *lms.z;
	xyz.y =  0.454369	*lms.x   + 0.473533	*lms.y + 0.0720978 *lms.z;
	xyz.z = -0.00962761	*lms.x - 0.00569803	*lms.y + 1.01533   *lms.z;
	
	return xyz;
}

Vector3 BlackBodyRadiation::XYZtoRGB(const Vector3 &xyz)
{
	Vector3 rgb(0.0, 0.0, 0.0);

	// sRGB
	rgb.x = xyz.x * 3.2410	+ xyz.y * -1.5374	+ xyz.z * -0.4986;
	rgb.y = xyz.x * -0.9692	+ xyz.y * 1.8760	+ xyz.z * 0.0416;
	rgb.z = xyz.x * 0.0556	+ xyz.y * -0.2040	+ xyz.z * 1.0570;

	/*double maxRGB = std::max(rgb.x, std::max(rgb.y, rgb.z));

	if (maxRGB > 0.0)
	{
		rgb.x /= maxRGB;
		rgb.y /= maxRGB;
		rgb.z /= maxRGB;
	}*/

	// Gamma Correction
	//rgb.x = powf(rgb.x, 1.0 / 2.4);
	//rgb.y = powf(rgb.y, 1.0 / 2.4);
	//rgb.z = powf(rgb.z, 1.0 / 2.4);

	return rgb;
}

const double LeScale = 0.25; // scale how much intensity the surrounding should reflect
void BlackBodyRadiation::draw(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke)
{
	//def i world coord
	Vector3 lookAt(2, 2, 2);
	Vector3 eyepos(2, 2, 7.5);
	eyepos -= lookAt;
	eyepos.rotY(6.28 * angle);
	eyepos += lookAt;
	angle += FirePresets::dt*0.25;

	// Set up camera (Not a very good way to do it though)
	Vector3 up(0.0, 1.0, 0.0);
	Vector3 forward = lookAt - eyepos;
	forward.normalize();
	Vector3 right = forward.cross(&up);
	right.normalize();
	up = right.cross(&forward);
	up.normalize();

	Vector3 minCoord, maxCoord;
	temperatureGrid.indexToWorld(0, 0, 0, minCoord.x, minCoord.y, minCoord.z);
	temperatureGrid.indexToWorld(temperatureGrid.xdim()-1, temperatureGrid.ydim()-1, temperatureGrid.zdim()-1, maxCoord.x, maxCoord.y, maxCoord.z);

	const double nearPlaneDistance = 0.1;
	const double aspect_ratio = double(xsize)/double(ysize);
	const double fovy = PI*0.25;
	const double w = nearPlaneDistance*tan(fovy);
	const double h = w/aspect_ratio;

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel
	{
		if(!FirePresets::QUALITY_ROOM)
		{
			// reset the mean radiance value
			#pragma omp for
			for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
			{
				LeMean[i] = 0.0;
			}
		}

		// Calculate the radiance value for each voxel, and the mean radiance value for all voxels
		#pragma omp for
		for(int x = 0; x < temperatureGrid.xdim(); ++x)
		{
			for(int y = 0; y < temperatureGrid.ydim(); ++y)
			{
				for(int z = 0; z < temperatureGrid.zdim(); ++z)
				{
					const double T = temperatureGrid.valueAtIndex(x, y, z);

					for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
					{
						int index = (x*temperatureGrid.ydim()*temperatureGrid.zdim() +
									y*temperatureGrid.zdim() +
									z)*FirePresets::TOTAL_SAMPLES + i;

						const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
						Le[index] = radiance(lambda, T);
						if(Le[index] < 0.0) Le[index] == 0.0;

						Le[index] = oa*Le[index]*temperatureGrid.dx()*temperatureGrid.dy()*temperatureGrid.dz();

						if(!FirePresets::QUALITY_ROOM)
						{
							LeMean[i] += Le[index];
							xm[i] += double(x)*Le[index];
							ym[i] += double(y)*Le[index];
							zm[i] += double(z)*Le[index];
						}

						
					}
				}
			}
		}

		if(!FirePresets::QUALITY_ROOM)
		{
			#pragma omp for
			for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
			{
				if(LeMean[i] > 0.0)
				{
					xm[i] /= LeMean[i];
					ym[i] /= LeMean[i];
					zm[i] /= LeMean[i];
				}
			}
		}

		double *local_L = &L[omp_get_thread_num()*FirePresets::TOTAL_SAMPLES];
		Vector3 normal = Vector3(0.0, 0.0, 1.0); //Not used yet, since we dont have a way to find the normal with the box yet.
		#pragma omp for
		for(int x = 0; x < xsize; ++x)
		{
			double u = -1.0 + double(x)*du; //Screenspace coordinate
			for(int y = 0; y < ysize; ++y)
			{
				double v = -1.0 + double(y)*dv;//Screenspace coordinate

				Vector3 nearPlanePos = eyepos + forward*nearPlaneDistance + right*u*(w/2) + up*v*(h/2);
				Vector3 direction = nearPlanePos-eyepos;
				direction.normalize();

				double tmin, tmax;
				Vector3 normal;

				int index = y*xsize + x;
				if(Vector3::rayBoxIntersection(minCoord, maxCoord, nearPlanePos, direction, &tmin, &tmax))
				{
					if(tmax > 0.0) //fluidbox is behind
					{
						Vector3 endPoint = nearPlanePos + direction*tmax;
						Vector3 startPoint;
						if(tmin > 0.0) // set startpoint at first intersection with box if camera is outside the box
							startPoint = nearPlanePos + direction*tmin;
						else 
							startPoint = nearPlanePos;

						// calculate the surrounding radiance
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
						{
							local_L[i] = 0.0;
							
							// using all voxels (very slow and incorrect, should use a monte-carlo raycasting instead)
							if(FirePresets::QUALITY_ROOM)
							{
								const int jump = 1;
								for(int a = 0; a < temperatureGrid.xdim(); a += jump)
								{
									for(int b = 0; b < temperatureGrid.ydim(); b += jump)
									{
										for(int c = 0; c < temperatureGrid.zdim(); c += jump)
										{
											int index = (a*temperatureGrid.ydim()*temperatureGrid.zdim() +
														 b*temperatureGrid.zdim() +
														 c)*FirePresets::TOTAL_SAMPLES + i;
								
											double xw2, yw2, zw2;
											temperatureGrid.indexToWorld(a, b, c, xw2, yw2, zw2);
											Vector3 p1(xw2, yw2, zw2);

											Vector3 diff = p1 - endPoint;
											double dist = diff.norm();
											//diff.normalize(); // we dont have the normal and therefor we dont use the diffuse term here
											//double diffuse = std::max(Vector3::dot(diff, normal), 0.0); 
											local_L[i] += pow(dist, -2.0)*Le[index]*LeScale/**diffuse*/;
										}
									}
								}
							}
							else // using only mean radiance value (fast but even more incorrect)
							{
								if(LeMean[i] > 0.0) 
								{
									double xw2, yw2, zw2;
									temperatureGrid.indexToWorld(xm[i], ym[i], zm[i], xw2, yw2, zw2);
									Vector3 p1(xw2, yw2, zw2);

									Vector3 diff = p1 - endPoint;
									double dist = diff.norm();
									//diff.normalize(); // we dont have the normal and therefor we dont use the diffuse term here
									//double diffuse = std::max(Vector3::dot(diff, normal), 0.0); 
									local_L[i] += pow(dist, -2.0)*LeMean[i]*LeScale/**diffuse*/;
								}
							}
						}

						//ray casting
						double length = (endPoint - startPoint).norm();
						double steps = length/wds;
						int intsteps = int(steps);
						Vector3 pos;
						for(int z = 0; z < intsteps; z += 1)
						{
							pos = endPoint - direction*double(z)*wds; //From end to start, we traverse the ray backwards here.

							if(temperatureGrid.worldIsValid(pos.x, pos.y, pos.z)) // check if pos is inside the grid
							{
								 double T = temperatureGrid.valueAtWorld(pos.x, pos.y, pos.z);

								// calculate the radiance for each wave length sample
								for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
								{
									const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
									local_L[i] = C*local_L[i] + oa*radiance(lambda, T)*wds;
								}
							}
						}
						double restLength = (pos - nearPlanePos).norm();
						double Crest = exp(-ot*restLength);
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1) // correct the intensity if startpoint is not at nearplane
							local_L[i] = Crest*local_L[i];

						// Calc XYZ-color from the radiance L
						Vector3 XYZ = Vector3(0.0);
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
						{
							int j = FirePresets::SAMPLE_STEP*i;
							XYZ.x += local_L[i] * CIE_X[j];
							XYZ.y += local_L[i] * CIE_Y[j];
							XYZ.z += local_L[i] * CIE_Z[j];
						}
						XYZ *= FirePresets::SAMPLE_DL;

						// Chromatic adaption of the light, high CHROMA value decreases the intensity of the light
						Vector3 LMS = XYZtoLMS(XYZ);
						LMS.x = LMS.x/(LMS.x + FirePresets::CHROMA);
						LMS.y = LMS.y/(LMS.y + FirePresets::CHROMA);
						LMS.z = LMS.z/(LMS.z + FirePresets::CHROMA);
						XYZ = LMStoXYZ(LMS);

						Vector3 rgb = XYZtoRGB(XYZ);
						image[index*3 + 0] = rgb.x; //R
						image[index*3 + 1] = rgb.y; //G
						image[index*3 + 2] = rgb.z; //B
					}
					else
					{
						image[index*3 + 0] = 0.0; //R
						image[index*3 + 1] = 0.0; //G
						image[index*3 + 2] = 0.0; //B
					}
				}
				else
				{
					image[index*3 + 0] = 0.0; //R
					image[index*3 + 1] = 0.0; //G
					image[index*3 + 2] = 0.0; //B
				}
			}
			#pragma omp critical 
			{
				printf("\rRender progress: %.02f%%, %.02fs, %d/%d threads", 1000.f*n++/temperatureGrid.xdim(), omp_get_wtime() - startTime, omp_get_num_threads(), omp_get_max_threads());
				fflush(stdout);
			}
		}
	}
	std::cout << "\n" << std::endl;

	// draw texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, xsize, ysize, 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
	glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
	glTexCoord2i(1, 1);		glVertex2f(1.0f, 1.0f);
	glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	
	glEnd();
}

// See draw for comments
void BlackBodyRadiation::drawColor(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke)
{
	//def i world coord
	Vector3 lookAt(2, 2, 2);
	Vector3 eyepos(2, 2, 7.5);
	eyepos -= lookAt;
	eyepos.rotY(6.28 * angle);
	eyepos += lookAt;
	angle += FirePresets::dt*0.25;
	funVar2 += 0.1;
	if(funVar2 >= 29.0) funVar2 = 5.0; // These wave lengths doesnt add much to the simulation... shouldn't be included at all
	funVar = (int)funVar2;

	// Start up the camera
	Vector3 up(0.0, 1.0, 0.0);
	Vector3 forward = lookAt - eyepos;
	forward.normalize();
	Vector3 right = forward.cross(&up);
	right.normalize();
	up = right.cross(&forward);
	up.normalize();

	Vector3 minCoord, maxCoord;
	temperatureGrid.indexToWorld(0, 0, 0, minCoord.x, minCoord.y, minCoord.z);
	temperatureGrid.indexToWorld(temperatureGrid.xdim()-1, temperatureGrid.ydim()-1, temperatureGrid.zdim()-1, maxCoord.x, maxCoord.y, maxCoord.z);

	const double nearPlaneDistance = 0.1;
	const double aspect_ratio = double(xsize)/double(ysize);
	const double fovy = PI*0.25;
	const double w = nearPlaneDistance*tan(fovy);
	const double h = w/aspect_ratio;

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel
	{
		// Reset the mean radiance value
		if(!FirePresets::QUALITY_ROOM)
		{
			#pragma omp for
			for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
			{
				LeMean[i] = 0.0;
			}
		}

		// calculate the radiance value for each voxel and the total mean radiance
		#pragma omp for
		for(int x = 0; x < temperatureGrid.xdim(); ++x)
		{
			for(int y = 0; y < temperatureGrid.ydim(); ++y)
			{
				for(int z = 0; z < temperatureGrid.zdim(); ++z)
				{
					const double T = temperatureGrid.valueAtIndex(x, y, z);

					int i = funVar;
					int index = (x*temperatureGrid.ydim()*temperatureGrid.zdim() +
							y*temperatureGrid.zdim() +
							z)*FirePresets::TOTAL_SAMPLES + i;

					const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;

					Le[index] = radiance(800e-9, T);
					Le[index] = oa*Le[index]*temperatureGrid.dx()*temperatureGrid.dy()*temperatureGrid.dz();
					
					if(Le[index] < 0.0) {
						std::cout << "Le ar negativ" << std::endl;
						Le[index] = 0.0;
					}

					if(!FirePresets::QUALITY_ROOM)
					{
						LeMean[i] += Le[index];
						xm[i] += double(x)*Le[index];
						ym[i] += double(y)*Le[index];
						zm[i] += double(z)*Le[index];
					}
				}
			}
		}

		if(!FirePresets::QUALITY_ROOM)
		{
			#pragma omp for
			for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
			{
				if(LeMean[i] > 0.0)
				{
					xm[i] /= LeMean[i];
					ym[i] /= LeMean[i];
					zm[i] /= LeMean[i];
				}
			}
		}

		double *local_L = &L[omp_get_thread_num()*FirePresets::TOTAL_SAMPLES];
		Vector3 normal = Vector3(0.0, 0.0, 1.0);
		#pragma omp for
		for(int x = 0; x < xsize; ++x)
		{
			double u = -1.0 + double(x)*du; //Screenspace coordinate
			for(int y = 0; y < ysize; ++y)
			{
				double v = -1.0 + double(y)*dv;//Screenspace coordinate

				Vector3 nearPlanePos = eyepos + forward*nearPlaneDistance + right*u*(w/2) + up*v*(h/2);
				Vector3 direction = nearPlanePos-eyepos;
				direction.normalize();

				double tmin, tmax;

				int index = y*xsize + x;
				if(Vector3::rayBoxIntersection(minCoord, maxCoord, nearPlanePos, direction, &tmin, &tmax))
				{
					if(tmax > 0.0) //fluidbox is behind
					{
						Vector3 endPoint = nearPlanePos + direction*tmax;
						Vector3 startPoint;
						if(tmin > 0.0)
							startPoint = nearPlanePos + direction*tmin;
						else
							startPoint = nearPlanePos;

						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
						{
							local_L[i] = 100000000.0;
							
							//Calc surround light with all voxels (VERY SLOW AND INCORRECT, should be some kind of monte-carlo raycasting here)
							if(FirePresets::QUALITY_ROOM)
							{
								const int jump = 1;
								for(int a = 0; a < temperatureGrid.xdim(); a += jump)
								{
									for(int b = 0; b < temperatureGrid.ydim(); b += jump)
									{
										for(int c = 0; c < temperatureGrid.zdim(); c += jump)
										{
											int index = (a*temperatureGrid.ydim()*temperatureGrid.zdim() +
														 b*temperatureGrid.zdim() +
														 c)*FirePresets::TOTAL_SAMPLES + i;
								
											double xw2, yw2, zw2;
											temperatureGrid.indexToWorld(a, b, c, xw2, yw2, zw2);
											Vector3 p1(xw2, yw2, zw2);

											Vector3 diff = p1 - endPoint;
											double dist = diff.norm();
											local_L[i] += pow(dist, -2.0)*Le[index]*LeScale;
										}
									}
								}
							}
							else // calculate surround light with the mean radiance value (FAST AND EVEN MORE INCORRECT)
							{	
								if(LeMean[i] > 0.0)
								{
									double xw2, yw2, zw2;
									temperatureGrid.indexToWorld(xm[i], ym[i], zm[i], xw2, yw2, zw2);
									Vector3 p1(xw2, yw2, zw2);

									Vector3 diff = p1 - endPoint;
									double dist = diff.norm();
									local_L[i] += pow(dist, -2.0)*LeMean[i]*LeScale;
								}
							}
						}

						double length = (endPoint - startPoint).norm();
						double steps = length/wds;
						int intsteps = int(steps);
						Vector3 pos;
						for(int z = 0; z < intsteps; z += 1)
						{
							pos = endPoint - direction*double(z)*wds; 

							if(temperatureGrid.worldIsValid(pos.x, pos.y, pos.z))
							{
								const double T = temperatureGrid.valueAtWorld(pos.x, pos.y, pos.z);

								for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
								{
									if(i == funVar) //Only give color from one specific sample
									{
										const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
										local_L[i] = C*local_L[i] + oa*radiance(800e-9, T)*double(FirePresets::SAMPLE_STEP)*wds;
									}
									else
										local_L[i] = C*local_L[i];
								}
							}
						}
						double restLength = (pos - nearPlanePos).norm();
						double Crest = exp(-ot*restLength);
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
							local_L[i] = Crest*local_L[i];

						Vector3 XYZ = Vector3(0.0);
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
						{
							int j = FirePresets::SAMPLE_STEP*i;
							XYZ.x += local_L[i] * CIE_X[j];
							XYZ.y += local_L[i] * CIE_Y[j];
							XYZ.z += local_L[i] * CIE_Z[j];
						}
						XYZ *= FirePresets::SAMPLE_DL;

						Vector3 LMS = XYZtoLMS(XYZ);
						LMS.x = LMS.x/(LMS.x + FirePresets::CHROMA);
						LMS.y = LMS.y/(LMS.y + FirePresets::CHROMA);
						LMS.z = LMS.z/(LMS.z + FirePresets::CHROMA);
						XYZ = LMStoXYZ(LMS);

						Vector3 rgb = XYZtoRGB(XYZ);
						image[index*3 + 0] = rgb.x; //R
						image[index*3 + 1] = rgb.y; //G
						image[index*3 + 2] = rgb.z; //B
					}
					else
					{
						image[index*3 + 0] = 0.0; //R
						image[index*3 + 1] = 0.0; //G
						image[index*3 + 2] = 0.0; //B
					}
				}
				else
				{
					image[index*3 + 0] = 0.0; //R
					image[index*3 + 1] = 0.0; //G
					image[index*3 + 2] = 0.0; //B
				}
			}
			#pragma omp critical 
			{
				printf("\rRender progress: %.02f%%, %.02fs, %d/%d threads", 1000.f*n++/temperatureGrid.xdim(), omp_get_wtime() - startTime, omp_get_num_threads(), omp_get_max_threads());
				fflush(stdout);
			}
		}
	}
	std::cout << "\n" << std::endl;

	// draw texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, xsize, ysize, 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
	glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
	glTexCoord2i(1, 1);		glVertex2f(1.0f, 1.0f);
	glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	
	glEnd();
}

// See draw for comments
void BlackBodyRadiation::drawFireBlueCore(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke)
{
	Vector3 lookAt(2, 2, 2);
	Vector3 eyepos(2, 2, 7.5);
	eyepos -= lookAt;
	eyepos.rotY(6.28 * angle);
	eyepos += lookAt;
	angle += FirePresets::dt*0.25;

	Vector3 up(0.0, 1.0, 0.0);
	Vector3 forward = lookAt - eyepos;
	forward.normalize();
	Vector3 right = forward.cross(&up);
	right.normalize();
	up = right.cross(&forward);
	up.normalize();

	Vector3 minCoord, maxCoord;
	temperatureGrid.indexToWorld(0, 0, 0, minCoord.x, minCoord.y, minCoord.z);
	temperatureGrid.indexToWorld(temperatureGrid.xdim()-1, temperatureGrid.ydim()-1, temperatureGrid.zdim()-1, maxCoord.x, maxCoord.y, maxCoord.z);

	const double nearPlaneDistance = 0.1;
	const double aspect_ratio = double(xsize)/double(ysize);
	const double fovy = PI*0.25;
	const double w = nearPlaneDistance*tan(fovy);
	const double h = w/aspect_ratio;

	int bluecore = 4;
	double bluecoredist = 0.5;

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel
	{
		if(!FirePresets::QUALITY_ROOM)
		{
			#pragma omp for
			for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
			{
				LeMean[i] = 0.0;
			}
		}

		#pragma omp for
		for(int x = 0; x < temperatureGrid.xdim(); ++x)
		{
			for(int y = 0; y < temperatureGrid.ydim(); ++y)
			{
				for(int z = 0; z < temperatureGrid.zdim(); ++z)
				{
					const double T = temperatureGrid.valueAtIndex(x, y, z);

					Vector3 pos;
					temperatureGrid.indexToWorld(x, y, z, pos.x, pos.y, pos.z);
					if(phi.grid->worldIsValid(pos.x, pos.y, pos.z) && phi.getCellType(pos.x, pos.y, pos.z) == FUEL)
					{
						// incorrect calculation of the surrounding light, since the blue core gets its energy from the heated gas. 
						// but it looks ok though
						int i = bluecore;
						int index = (x*temperatureGrid.ydim()*temperatureGrid.zdim() +
									y*temperatureGrid.zdim() +
									z)*FirePresets::TOTAL_SAMPLES + i;

						const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
						Le[index] = radiance(lambda, T);
						if(Le[index] < 0.0) Le[index] == 0.0;

						Le[index] = oa*Le[index]*temperatureGrid.dx()*temperatureGrid.dy()*temperatureGrid.dz();

						if(!FirePresets::QUALITY_ROOM)
						{
							LeMean[i] += Le[index];
							xm[i] += double(x)*Le[index];
							ym[i] += double(y)*Le[index];
							zm[i] += double(z)*Le[index];
						}
					}
					else
					{
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
						{
							int index = (x*temperatureGrid.ydim()*temperatureGrid.zdim() +
										y*temperatureGrid.zdim() +
										z)*FirePresets::TOTAL_SAMPLES + i;

							const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
							Le[index] = radiance(lambda, T);
							if(Le[index] < 0.0) Le[index] == 0.0;

							Le[index] = oa*Le[index]*temperatureGrid.dx()*temperatureGrid.dy()*temperatureGrid.dz();

							if(!FirePresets::QUALITY_ROOM)
							{
								LeMean[i] += Le[index];
								xm[i] += double(x)*Le[index];
								ym[i] += double(y)*Le[index];
								zm[i] += double(z)*Le[index];
							}
						}
					}
				}
			}
		}

		if(!FirePresets::QUALITY_ROOM)
		{
			#pragma omp for
			for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
			{
				if(LeMean[i] > 0.0)
				{
					xm[i] /= LeMean[i];
					ym[i] /= LeMean[i];
					zm[i] /= LeMean[i];
				}
			}
		}

		double *local_L = &L[omp_get_thread_num()*FirePresets::TOTAL_SAMPLES];
		Vector3 normal = Vector3(0.0, 0.0, 1.0);
		#pragma omp for
		for(int x = 0; x < xsize; ++x)
		{
			double u = -1.0 + double(x)*du; //Screenspace coordinate
			for(int y = 0; y < ysize; ++y)
			{
				double v = -1.0 + double(y)*dv;//Screenspace coordinate

				Vector3 nearPlanePos = eyepos + forward*nearPlaneDistance + right*u*(w/2) + up*v*(h/2);
				Vector3 direction = nearPlanePos-eyepos;
				direction.normalize();

				double tmin, tmax;
				Vector3 normal;

				int index = y*xsize + x;
				if(Vector3::rayBoxIntersection(minCoord, maxCoord, nearPlanePos, direction, &tmin, &tmax))
				{
					if(tmax > 0.0) //fluidbox is behind
					{
						Vector3 endPoint = nearPlanePos + direction*tmax;
						Vector3 startPoint;
						if(tmin > 0.0)
							startPoint = nearPlanePos + direction*tmin;
						else
							startPoint = nearPlanePos;

						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
						{
							local_L[i] = 0.0;
							
							if(FirePresets::QUALITY_ROOM)
							{
								const int jump = 1;
								for(int a = 0; a < temperatureGrid.xdim(); a += jump)
								{
									for(int b = 0; b < temperatureGrid.ydim(); b += jump)
									{
										for(int c = 0; c < temperatureGrid.zdim(); c += jump)
										{
											int index = (a*temperatureGrid.ydim()*temperatureGrid.zdim() +
														 b*temperatureGrid.zdim() +
														 c)*FirePresets::TOTAL_SAMPLES + i;
								
											double xw2, yw2, zw2;
											temperatureGrid.indexToWorld(a, b, c, xw2, yw2, zw2);
											Vector3 p1(xw2, yw2, zw2);

											Vector3 diff = p1 - endPoint;
											double dist = diff.norm();
											local_L[i] += pow(dist, -2.0)*Le[index]*LeScale;
										}
									}
								}
							}
							else
							{
								if(LeMean[i] > 0.0) 
								{
									double xw2, yw2, zw2;
									temperatureGrid.indexToWorld(xm[i], ym[i], zm[i], xw2, yw2, zw2);
									Vector3 p1(xw2, yw2, zw2);

									Vector3 diff = p1 - endPoint;
									double dist = diff.norm();
									local_L[i] += pow(dist, -2.0)*LeMean[i]*LeScale;
								}
							}
						}

						double length = (endPoint - startPoint).norm();
						double steps = length/wds;
						int intsteps = int(steps);
						Vector3 pos;
						for(int z = 0; z < intsteps; z += 1)
						{
							pos = endPoint - direction*double(z)*wds; //From end to start we traverse the ray backwards here.

							if(temperatureGrid.worldIsValid(pos.x, pos.y, pos.z))
							{
								const double T = temperatureGrid.valueAtWorld(pos.x, pos.y, pos.z);

								if(phi.grid->worldIsValid(pos.x, pos.y, pos.z) && phi.getCellType(pos.x, pos.y, pos.z) == FUEL)
								{
									for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
									{
										// if in bluecore try to simulate some kind of molecular photon reaction with the photon 
										// and converge the intensity from all the samples to one specific sample
										if(i == bluecore) 
										{
											double Lsum = 0.0;
											for(int j = 0; j < FirePresets::TOTAL_SAMPLES; ++j)
											{
												if(j != bluecore)
												{
													double amount = wds/bluecoredist;
													if(amount > 1.0) amount = 1.0;
													amount *= double(rand()%10001)/10000.0;
													Lsum += C*local_L[j]*amount;
													local_L[j] = C*local_L[j]*(1.0 - amount);
												}
											}
											local_L[i] += Lsum;
										}
										else
										{
											local_L[i] = C*local_L[i];
										}
									}
								}
								else
								{
									for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
									{
										const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
										local_L[i] = C*local_L[i] + oa*radiance(lambda, T)*wds;
									}
								}
							}
							else 
							{
								for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
								{
									local_L[i] = C*local_L[i];
								}
							}
						}
						double restLength = (pos - nearPlanePos).norm();
						double Crest = exp(-ot*restLength);
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
							local_L[i] = Crest*local_L[i];

						Vector3 XYZ = Vector3(0.0);
						for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
						{
							int j = FirePresets::SAMPLE_STEP*i;
							XYZ.x += local_L[i] * CIE_X[j];
							XYZ.y += local_L[i] * CIE_Y[j];
							XYZ.z += local_L[i] * CIE_Z[j];
						}
						XYZ *= FirePresets::SAMPLE_DL;

						Vector3 LMS = XYZtoLMS(XYZ);
						LMS.x = LMS.x/(LMS.x + FirePresets::CHROMA);
						LMS.y = LMS.y/(LMS.y + FirePresets::CHROMA);
						LMS.z = LMS.z/(LMS.z + FirePresets::CHROMA);
						XYZ = LMStoXYZ(LMS);

						Vector3 rgb = XYZtoRGB(XYZ);
						image[index*3 + 0] = rgb.x; //R
						image[index*3 + 1] = rgb.y; //G
						image[index*3 + 2] = rgb.z; //B
					}
					else
					{
						image[index*3 + 0] = 0.0; //R
						image[index*3 + 1] = 0.0; //G
						image[index*3 + 2] = 0.0; //B
					}
				}
				else
				{
					image[index*3 + 0] = 0.0; //R
					image[index*3 + 1] = 0.0; //G
					image[index*3 + 2] = 0.0; //B
				}
			}
			#pragma omp critical 
			{
				printf("\rRender progress: %.02f%%, %.02fs, %d/%d threads", 1000.f*n++/temperatureGrid.xdim(), omp_get_wtime() - startTime, omp_get_num_threads(), omp_get_max_threads());
				fflush(stdout);
			}
		}
	}
	std::cout << "\n" << std::endl;

	//rita ut textur
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, xsize, ysize, 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
	glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
	glTexCoord2i(1, 1);		glVertex2f(1.0f, 1.0f);
	glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	
	glEnd();
}

// See draw for comments
void BlackBodyRadiation::drawBlueCore(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke)
{
	//def i world coord
	Vector3 lookAt(2, 2, 2);
	Vector3 eyepos(2, 2, 7.5);
	eyepos -= lookAt;
	eyepos.rotY(6.28 * angle);
	eyepos += lookAt;
	angle += FirePresets::dt*5.0;

	Vector3 up(0.0, 1.0, 0.0);
	Vector3 forward = lookAt - eyepos;
	forward.normalize();
	Vector3 right = forward.cross(&up);
	right.normalize();
	up = right.cross(&forward);
	up.normalize();

	Vector3 minCoord, maxCoord;
	temperatureGrid.indexToWorld(0, 0, 0, minCoord.x, minCoord.y, minCoord.z);
	temperatureGrid.indexToWorld(temperatureGrid.xdim()-1, temperatureGrid.ydim()-1, temperatureGrid.zdim()-1, maxCoord.x, maxCoord.y, maxCoord.z);

	const double nearPlaneDistance = 0.1;
	const double aspect_ratio = double(xsize)/double(ysize);
	const double fovy = PI*0.25;
	const double w = nearPlaneDistance*tan(fovy);
	const double h = w/aspect_ratio;

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel
	{
		Vector3 normal = Vector3(0.0, 0.0, 1.0);
		#pragma omp for
		for(int x = 0; x < xsize; ++x)
		{
			double u = -1.0 + double(x)*du; //Screenspace coordinate
			for(int y = 0; y < ysize; ++y)
			{
				double v = -1.0 + double(y)*dv;//Screenspace coordinate

				Vector3 nearPlanePos = eyepos + forward*nearPlaneDistance + right*u*(w/2) + up*v*(h/2);
				Vector3 direction = nearPlanePos-eyepos;
				direction.normalize();

				double tmin, tmax;
				Vector3 normal;

				int index = y*xsize + x;
				if(Vector3::rayBoxIntersection(minCoord, maxCoord, nearPlanePos, direction, &tmin, &tmax))
				{
					if(tmax > 0.0) //fluidbox is behind
					{
						Vector3 endPoint = nearPlanePos + direction*tmax;
						Vector3 startPoint;
						if(tmin > 0.0)
							startPoint = nearPlanePos + direction*tmin;
						else
							startPoint = nearPlanePos;

						//ray casting
						double length = (endPoint - startPoint).norm();
						double steps = length/wds;
						int intsteps = int(steps); 
						Vector3 pos;
						double b = 0.0;
						double samples = 0.0;
						for(int z = 0; z < intsteps; z += 1)
						{
							pos = endPoint - direction*double(z)*wds; //From end to start we traverse the ray backwards here.

							// if hit the bluecore, calculate the diffuse light with the surface
							if(phi.grid->worldIsValid(pos.x, pos.y, pos.z) && phi.getCellType(pos.x, pos.y, pos.z) == FUEL)
							{
								Vector3 N = phi.getNormal(pos.x, pos.y, pos.z);
								N.normalize();

								b = std::max(-Vector3::dot(N, direction), 0.0);
								break;
							}
						}

						image[index*3 + 0] = 0; //R
						image[index*3 + 1] = 0; //G
						image[index*3 + 2] = b; //B
					}
					else
					{
						image[index*3 + 0] = 0.0; //R
						image[index*3 + 1] = 0.0; //G
						image[index*3 + 2] = 0.0; //B
					}
				}
				else
				{
					image[index*3 + 0] = 0.0; //R
					image[index*3 + 1] = 0.0; //G
					image[index*3 + 2] = 0.0; //B
				}
			}
			#pragma omp critical 
			{
				printf("\rRender progress: %.02f%%, %.02fs, %d/%d threads", 1000.f*n++/temperatureGrid.xdim(), omp_get_wtime() - startTime, omp_get_num_threads(), omp_get_max_threads());
				fflush(stdout);
			}
		}
	}
	std::cout << "\n" << std::endl;

	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, xsize, ysize, 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
	glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
	glTexCoord2i(1, 1);		glVertex2f(1.0f, 1.0f);
	glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	
	glEnd();
}