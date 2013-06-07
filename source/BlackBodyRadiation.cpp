#include "BlackBodyRadiation.h"
#include <stdio.h>
#include <cmath>
#include "Vector3.h"
#include "firePresets.h"

#include "GridField.hpp"
#include "LevelSet.h"

#include "SmokeDensity.h"

#include <omp.h>

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
}

//Def xsize och gridx först osv.
void BlackBodyRadiation::allocate()
{
	dx = 1.0/double(xsize);
	dy = 1.0/double(ysize);
	dz = 1.0/double(zsize);

	wds = double(gridz)/double(gridx)*double(FirePresets::GRID_SIZE)/double(zsize); //fysisk steglängd mellan sampel

	oa = 0.05; //absorberings koef, för rapport 1 använd 0.01
	os = 0.0; //scattering koef
	ot = oa + os; //tot
	C = exp(-ot*wds);

	const int IMSIZE= xsize*ysize*3;
	image = new GLfloat[IMSIZE];
	#define GL_CLAMP_TO_EDGE 0x812F
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//Används för att rendera rummet, kan nog vara lägre samplad än total_samples, alt gör man denna med macgrid för slippa göra samma beräkningar 2 ggr
	//dock tror jag det kostar förmycket minne
	int LeSize = gridx*gridy*gridz*FirePresets::TOTAL_SAMPLES;
	Le = new double[LeSize]; 
	
	L = new double[omp_get_max_threads()*FirePresets::TOTAL_SAMPLES];
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

void BlackBodyRadiation::draw(const GridField<double> &temperatureGrid, const LevelSet &phi, const SmokeDensity &smoke)
{
	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel
	{
		#pragma omp for
		for(int x = 0; x < temperatureGrid.xdim(); ++x)
		{
			for(int y = 0; y < temperatureGrid.ydim(); ++y)
			{
				for(int z = 0; z < temperatureGrid.zdim(); ++z)
				{
					const double T = temperatureGrid.valueAtIndex(x, y, z);

					//Räkna ut intensitet för varje våglängd
					for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
					{
						int index = (x*temperatureGrid.ydim()*temperatureGrid.zdim() +
									y*temperatureGrid.zdim() +
									z)*FirePresets::TOTAL_SAMPLES + i;

						const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
						Le[index] = oa*radiance(lambda, T)*wds;
					}
				}
			}
		}

		double *local_L = &L[omp_get_thread_num()*FirePresets::TOTAL_SAMPLES];
		Vector3 normal = Vector3(0.0, 0.0, 1.0);
		#pragma omp for
		for(int x = 0; x < xsize; ++x)
		{
			for(int y = 0; y < ysize; ++y)
			{
				double xw1, yw1, zw1;
				temperatureGrid.localToWorld(dx*double(x), dy*double(y), 0.0, xw1, yw1, zw1);
				Vector3 p0(xw1, yw1, zw1);

				for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
				{
					local_L[i] = 0.0;

					//Calc surround light with all voxels
					const int jump = 5;
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

								Vector3 diff = p1 - p0;
								double dist = diff.norm();
								diff.normalize();
								double diffuse = std::max(Vector3::dot(diff, normal), 0.0);

								local_L[i] += pow(dist, -2.0)*Le[index]*diffuse*0.4;
							}
						}
					}
				}

				for(int z = 0; z < zsize; ++z)
				{
					double xw, yw, zw;
					temperatureGrid.localToWorld(dx*double(x), dy*double(y), dz*double(z), xw, yw, zw);
					const double T = temperatureGrid.valueAtWorld(xw, yw, zw);

					//Räkna ut intensitet för varje våglängd
					for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i += 1)
					{
						const double lambda = (360.0 + double(i*FirePresets::SAMPLE_STEP)*5)*1e-9;
						local_L[i] = C*local_L[i] + oa*radiance(lambda, T)*wds;
					}
				}

				//Beräkna XYZ från L
				Vector3 XYZ = Vector3(0.0);
				for(int i = 0; i < FirePresets::TOTAL_SAMPLES; i+= 1)
				{
					int j = FirePresets::SAMPLE_STEP*i;
					XYZ.x += local_L[i] * CIE_X[j];
					XYZ.y += local_L[i] * CIE_Y[j];
					XYZ.z += local_L[i] * CIE_Z[j];
				}
				XYZ *= FirePresets::SAMPLE_DL;

				//en variant av chromatic adaption, högt värde på crom minskar intensiteten, lågt värde ökar den.
				Vector3 LMS = XYZtoLMS(XYZ);
				LMS.x = LMS.x/(LMS.x + FirePresets::CHROMA);
				LMS.y = LMS.y/(LMS.y + FirePresets::CHROMA);
				LMS.z = LMS.z/(LMS.z + FirePresets::CHROMA);
				XYZ = LMStoXYZ(LMS);

				Vector3 rgb = XYZtoRGB(XYZ);

				int index = y*xsize + x; // Hitta index i texturen för x och y koordinat.
				image[index*3 + 0] = rgb.x; //R
				image[index*3 + 1] = rgb.y; //G
				image[index*3 + 2] = rgb.z; //B
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
	
	if(temperatureGrid.xdim() > temperatureGrid.ydim())
	{
		float aspect = float(temperatureGrid.ydim())/float(temperatureGrid.xdim());
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(1.0f, aspect);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, aspect);	
	}
	else
	{
		float aspect = float(temperatureGrid.xdim())/float(temperatureGrid.ydim());
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(aspect, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(aspect, 1.0f);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	}
	
	glEnd();

	drawLevelSet(phi);
}

void BlackBodyRadiation::drawLevelSet(const LevelSet &phi)
{
	const int IMSIZE= phi.grid->xdim()*phi.grid->ydim()*3;
	GLfloat *image = new GLfloat[IMSIZE];
	GLuint textureID;
	#define GL_CLAMP_TO_EDGE 0x812F
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel for
	for(int x = 0; x < phi.grid->xdim(); ++x)
	{
		for(int y = 0; y < phi.grid->ydim(); ++y)
		{
			CellType type = BURNT;
			double vmin = FLT_MAX;
			for(int z = 0; z < phi.grid->zdim(); ++z)
			{
				if(phi.getCellType(x, y, z) == FUEL)
				{
					type = FUEL;
					break;
				}
				else if(vmin > -phi.grid->valueAtIndex(x, y, z))
				{
					vmin = -phi.grid->valueAtIndex(x, y, z);
				}
			}

			int index = y*phi.grid->xdim() + x; // Hitta index i texturen för x och y koordinat.

			if(type == FUEL)
			{
				image[index*3 + 0] = 0; //R
				image[index*3 + 1] = 0; //G
				image[index*3 + 2] = 1; //B
			}
			else
			{
				image[index*3 + 0] = vmin; //R
				image[index*3 + 1] = vmin; //G
				image[index*3 + 2] = 0; //B
			}
		}
		/*#pragma omp critical 
		{
			printf("\rRender progress: %.02f%%, %.02fs, %d/%d threads", 1000.f*n++/temperatureGrid.xdim(), omp_get_wtime() - startTime, omp_get_num_threads(), omp_get_max_threads());
			fflush(stdout);
		}*/
	}
	std::cout << "\n" << std::endl;

	//rita ut textur
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, phi.grid->xdim(), phi.grid->ydim(), 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	if(phi.grid->xdim() > phi.grid->ydim())
	{
		float aspect = double(phi.grid->ydim())/double(phi.grid->xdim());
		glTexCoord2i(0, 0);		glVertex2f(0.0f, aspect);
		glTexCoord2i(1, 0);		glVertex2f(1.0f, aspect);
		glTexCoord2i(1, 1);		glVertex2f(1.0f, 1.0);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0);	
	}
	else
	{
		float aspect = double(phi.grid->xdim())/double(phi.grid->ydim());
		glTexCoord2i(0, 0);		glVertex2f(aspect, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(1.0, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(1.0, 1.0f);
		glTexCoord2i(0, 1);		glVertex2f(aspect, 1.0f);	
	}
	
	glEnd();

	 //TODO DEALLOKERA OCH ALLOKERA PÅ BÄTTRE STÄLLE!
	delete[] image;
	glDeleteTextures( 1, &textureID );
}

void BlackBodyRadiation::drawSmoke(const SmokeDensity &smoke)
{
	//TODO Placera denna allokering på ett annat ställe, samt deallokeringen.
	const int XSIZE = 300;
	const int YSIZE = 600;
	const int ZSIZE = smoke.grid->zdim()*2.0;
	const int IMSIZE= XSIZE*YSIZE*3;
	GLfloat *image = new GLfloat[IMSIZE];
	GLuint textureID;
	#define GL_CLAMP_TO_EDGE 0x812F
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	const double dx = 1.0/double(XSIZE);
	const double dy = 1.0/double(YSIZE);
	const double dz = 1.0/double(ZSIZE);

	const double wdz = double(smoke.grid->zdim())/double(smoke.grid->xdim())*double(FirePresets::GRID_SIZE)/double(ZSIZE); //fysisk steglängd mellan sampel

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel for
	for(int x = 0; x < XSIZE; ++x)
	{
		for(int y = 0; y < YSIZE; ++y)
		{
			double mdens = 0.0;
			for(int z = 0; z < ZSIZE; ++z)
			{
				double xw, yw, zw;
				smoke.grid->localToWorld(dx*double(x), dy*double(y), dz*double(z), xw, yw, zw);
				const double d = smoke.grid->valueAtWorld(xw, yw, zw);
				if(d > mdens)
					mdens = d;
			}

			int index = y*XSIZE + x; // Hitta index i texturen för x och y koordinat.
			image[index*3 + 0] = 0.0; //R
			image[index*3 + 1] = mdens; //G
			image[index*3 + 2] = 0.0; //B
		}
	}

	//rita ut textur
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, XSIZE, YSIZE, 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	if(smoke.grid->xdim() > smoke.grid->ydim())
	{
		float aspect = float(smoke.grid->ydim())/float(smoke.grid->xdim());
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(1.0f, aspect);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, aspect);	
	}
	else
	{
		float aspect = float(smoke.grid->xdim())/float(smoke.grid->ydim());
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(aspect, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(aspect, 1.0f);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	}
	
	glEnd();

	 //TODO DEALLOKERA OCH ALLOKERA PÅ BÄTTRE STÄLLE!
	delete[] image;
	glDeleteTextures( 1, &textureID );
}

//Per voxel rendering
/*
void BlackBodyRadiation::draw(const GridField<double> &temperatureGrid, const LevelSet &phi)
{
	const float xdim = temperatureGrid.xdim();
	const float ydim = temperatureGrid.ydim();
	const float zdim = temperatureGrid.zdim();

	const double oa = 0.01; //absorberings koef, för rapport 1 använd 0.01
	const double os = 0.0; //scattering koef
	const double ot = oa + os; //tot
	const double C = exp(-ot*FirePresets::dx);


	const int STEPSIZE = 1;
	const int SAMPLES = 89;//Antal samplade våglängder
	const double dl = 5e-9*double(STEPSIZE);//dx för våglängderna
	double L[SAMPLES];

	//TODO Placera denna allokering på ett annat ställe, samt deallokeringen.
	const int IMSIZE= temperatureGrid.xdim()*temperatureGrid.ydim()*3;
	GLfloat *image = new GLfloat[IMSIZE];
	GLuint textureID;
	#define GL_CLAMP_TO_EDGE 0x812F
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel for private(L)
	for(int x = 0; x < temperatureGrid.xdim(); ++x)
	{
		for(int y = 0; y < temperatureGrid.ydim(); ++y)
		{
			for(int i = 0; i < SAMPLES; ++i)
				L[i] = 0.0;

			for(int z = 0; z < temperatureGrid.zdim(); ++z)
			{
				const double T = temperatureGrid.valueAtIndex(x, y, z);

				//Räkna ut intensitet för varje våglängd
				for(int i = 0; i < SAMPLES; i += STEPSIZE)
				{
					const double lambda = (360.0 + double(i)*5)*1e-9;
					L[i] = C*L[i] + oa*radiance(lambda, T)*FirePresets::dx;
					//L[i] = C*L[i] + (1.0 - C)*radiance(lambda, T)/ot; 
				}
			}

			//Beräkna XYZ från L
			Vector3 XYZ = Vector3(0.0);
			for(int i = 0; i < SAMPLES; i+= STEPSIZE)
			{
				XYZ.x += L[i] * CIE_X[i];
				XYZ.y += L[i] * CIE_Y[i];
				XYZ.z += L[i] * CIE_Z[i];
			}
			XYZ *= dl;

			//en variant av chromatic adaption, högt värde på crom minskar intensiteten, lågt värde ökar den.
			Vector3 LMS = XYZtoLMS(XYZ);
			const double crom = 1.0;
			LMS.x = LMS.x/(LMS.x + crom);
			LMS.y = LMS.y/(LMS.y + crom);
			LMS.z = LMS.z/(LMS.z + crom);
			XYZ = LMStoXYZ(LMS);

			Vector3 rgb = XYZtoRGB(XYZ);

			int index = y*temperatureGrid.xdim() + x; // Hitta index i texturen för x och y koordinat.
			image[index*3 + 0] = rgb.x; //R
			image[index*3 + 1] = rgb.y; //G
			image[index*3 + 2] = rgb.z; //B
		}
		#pragma omp critical 
		{
			printf("\rRender progress: %.02f%%, %.02fs, %d/%d threads", 1000.f*n++/temperatureGrid.xdim(), omp_get_wtime() - startTime, omp_get_num_threads(), omp_get_max_threads());
			fflush(stdout);
		}
	}
	std::cout << "\n" << std::endl;

	//rita ut textur
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, temperatureGrid.xdim(), temperatureGrid.ydim(), 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	if(temperatureGrid.xdim() > temperatureGrid.ydim())
	{
		float aspect = ydim/xdim;
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(1.0f, aspect);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, aspect);	
	}
	else
	{
		float aspect = xdim/ydim;
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(aspect, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(aspect, 1.0f);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	}
	
	glEnd();

	 //TODO DEALLOKERA OCH ALLOKERA PÅ BÄTTRE STÄLLE!
	delete[] image;
	glDeleteTextures( 1, &textureID );
}
*/

	//RITA UT FUEL OCH BURNT
	//TODO TA BORT SENARE NÄR DENNA INTE BEHÖVS FÖR TESTNING LÄNGRE
	/* 
void BlackBodyRadiation::draw(const GridField<double> &temperatureGrid, const LevelSet &phi)
{
	const float xdim = temperatureGrid.xdim();
	const float ydim = temperatureGrid.ydim();
	const float zdim = temperatureGrid.zdim();

	//TODO Placera denna allokering på ett annat ställe, samt deallokeringen.
	const int IMSIZE= temperatureGrid.xdim()*temperatureGrid.ydim()*3;
	GLfloat *image = new GLfloat[IMSIZE];
	GLuint textureID;
	#define GL_CLAMP_TO_EDGE 0x812F
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	float startTime = omp_get_wtime();
	int n = 0;
	#pragma omp parallel for
	for(int x = 0; x < temperatureGrid.xdim(); ++x)
	{
		for(int y = 0; y < temperatureGrid.ydim(); ++y)
		{
			Vector3 rgb = Vector3(0.0);
			for(int z = 0; z < temperatureGrid.zdim(); ++z)
			{
				if(phi.getCellType(x, y, z) == FUEL)
				{
					rgb.z += 20.0;
				}
				else
				{
					rgb.x += 1.0;
					rgb.y += 1.0;
				}
			}

			rgb *= 1.0/double(zdim);

			int index = y*temperatureGrid.xdim() + x; // Hitta index i texturen för x och y koordinat.
			image[index*3 + 0] = rgb.x; //R
			image[index*3 + 1] = rgb.y; //G
			image[index*3 + 2] = rgb.z; //B
		}
		#pragma omp critical 
		{
			printf("\rRender progress: %.02f%%, %.02fs, %d/%d threads", 1000.f*n++/temperatureGrid.xdim(), omp_get_wtime() - startTime, omp_get_num_threads(), omp_get_max_threads());
			fflush(stdout);
		}
	}
	std::cout << "\n" << std::endl;

	//rita ut textur
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16, temperatureGrid.xdim(), temperatureGrid.ydim(), 0, GL_RGB, GL_FLOAT, image); 
	glBegin(GL_QUADS);
	
	if(temperatureGrid.xdim() > temperatureGrid.ydim())
	{
		float aspect = ydim/xdim;
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(1.0f, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(1.0f, aspect);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, aspect);	
	}
	else
	{
		float aspect = xdim/ydim;
		glTexCoord2i(0, 0);		glVertex2f(0.0f, 0.0f);
		glTexCoord2i(1, 0);		glVertex2f(aspect, 0.0f);
		glTexCoord2i(1, 1);		glVertex2f(aspect, 1.0f);
		glTexCoord2i(0, 1);		glVertex2f(0.0f, 1.0f);	
	}
	
	glEnd();

	 //TODO DEALLOKERA OCH ALLOKERA PÅ BÄTTRE STÄLLE!
	delete[] image;
	glDeleteTextures( 1, &textureID );
}*/