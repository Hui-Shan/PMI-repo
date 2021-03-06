#define _USE_MATH_DEFINES
#include <cmath>
#include "math.h"


#ifndef CONVOLUTIONIMAGEFILTER_H
#define CONVOLUTIONIMAGEFILTER_H

#include "ImageFilter.h"


namespace hmc {
	class ConvolutionImageFilter : public ImageFilter
	{

	public:
		// Constructor.
		//ConvolutionImageFilter() : _rad(0) {};
		ConvolutionImageFilter(int rad=0) : _rad(rad) {};

		// Destructor. This class does not own any free store allocated objects; 
		// but derived classes might, so it's virtual and empty
		virtual ~ConvolutionImageFilter() {};

		
		/*// Set the input image for this filter
		void setInput(const vector<T>& i) {
			// The input should be const. However, we cannot store it that way in our
			// base class. Therefor we cast away the const and store a pointer to the
			// original data. Upon using the input data in update(), we cast it back.
			_input = const_cast<vector<T>*>(&i);
		}

		// Get the input back as a const ref
		const vector<T>& getInput() const {
			// Didn't touch _input, cast it back to const;
			return const_cast<const vector<T>&>(*_input);
		}*/

		// Sets the radius of the Gaussian 
		void setRadius(const int radius) { _rad = radius; };


/*		// Get the output image result of this filter; 
		// available after calling update()
		vector<T> getOutput() const { return _output; };
		// Update the image filter and compute the output
		virtual void update()
		{
			// Didn't touch _input, cast it back to const; then execute()
			execute(getInput());
		}
*/
	protected:		

		// This method should be overloaded in your derived class and implement the
		// image filter that fills _output		
		void execute(const Image& im) {			
			
			// Make the output a zero valued image of size of the input
			Image::dimension imdim = im.size();
			
			Image i2 = Image(imdim);
			cout << "i2(0, 0, 0, 0, 0)" << i2(0, 0, 0, 0, 0) << "\n";

			_output = Image(imdim);
			cout << "_output(0, 0, 0, 0, 0)=" <<  _output(0, 0, 0, 0, 0) << " im(0, 0, 0, 0, 0)=" << im(0, 0, 0, 0, 0) << "\n";
			
			//for (int x : _output.size()) cout << x << " ";
			//cout << "\n";

			// Get all the dimensions
			int dimx = imdim[0];
			int dimy = imdim[1];
			int dimz = imdim[2];
			int dimc = imdim[3];
			int dimt = imdim[4];
			
			for (unsigned int t{ 0 }; t < dimt; ++t) {				
				for (unsigned int c{ 0 }; c < dimc; ++c) {
					for (unsigned int z{ 0 }; z < dimz; ++z) {
						for (unsigned int y{ 0 }; y < dimy; ++y) {
							for (unsigned int x{ 0 }; x < dimx; ++x) {
								
								/*
								//int index = int(_output.get_data_index(x, y, z, c, t));
								T vox_val = im(x, y, z, c, t);																								
								T vox_sum = 0.0;
								double gauss_val;
								
								for (int zi = -_rad; zi <= _rad; ++zi) {								
									for (int yi = -_rad; yi <= _rad; ++yi) {
										for (int xi = -_rad; xi <= _rad; ++xi) {																						
											gauss_val = gauss3D(xi, yi, zi, _rad);											
											vox_sum += T(vox_val * gauss_val);																						
											//cout << gauss_val << " " << vox_val << " " << vox_sum << "\n";
											if (zi == 0 & yi == 0 & xi == 0 & (x%20)==0 & (y%20)==0 & (z%20)==0)
												cout << gauss_val << " " << vox_sum << " || ";
										}										
									}
								}*/

								if (x % 50 == 0 & y % 50 == 0 & z % 50 == 0) {
									cout << "pre:  " << im(x, y, z, c, t) << " " << _output(x, y, z, c, t) << "\n ";// << vox_val << " vox sum " << T(vox_sum) << "\n";
								}
								
								_output(x, y, z, c, t) = T(x + y + z + c + t);// im(x, y, z, c, t);
								cout << _output(x, y, z, c, t) << "address " << &(_output(x, y, z, c, t)) << "\n";
								
								if (x % 50 == 0 & y % 50 == 0 & z % 50 == 0) {							
									cout << "post: " << im(x, y, z, c, t) << " " << _output(x, y, z, c, t) << "\n ";// << vox_val << " vox sum " << T(vox_sum) << "\n";
								}/*
								//	cout << x << "," << y << "," << z << ", " << c << ", " << t << "\n";
								//	cout << im(x,y,z,c,t) << " " << vox_sum << " " << num_in_range << "\n";
								//}								
								_output(x, y, z, c, t) = 0;// vox_sum / double(pow(2 * _rad + 1, 3)); // im(x, y, z, c, t)*0.5*gauss3D(0, 0, 0, _rad);// vox_sum;
								if (_output(x, y, z, c, t) != im(x, y, z, c, t)) {
									cout << x << " " << y << " " << z << " " << c << " " << t << "\n";
								}*/
								//_output(x, y, z, c, t) = im(x, y, z, c, t); // vox_sum;// vox_sum / double(pow(2 * _rad + 1, 3)); // im(x, y, z, c, t)*0.5*gauss3D(0, 0, 0, _rad);// vox_sum;
								
							}
						}
					}
				}
			}
			
			//cout << "num voxels " << voxls; 


		};

		//T get_pixel_value(int x, int y, int z, int xDim, int yDim) {
		//	return (*_input)[(z * xDim + yDim) + (y * xDim) + x];
		//}

	private:
		// Temporary storage of a pointer to the image data. Because we do some
		// const-magic, we keep it private.		
		int _rad;

		Image _kernel; 
		
		// Function to return the Gaussian kernel value for voxel (xi, yi, zi) wrt
		// center voxel (x0, y0, z0)
		double gauss3D(int dx, int dy, int dz, int rad=1.0) {			
			double val = 1.0;
			if (rad > 0) {
				double factor = 1 / pow(sqrt(2 * M_PI * rad), 3);
				double numerator = double(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
				double denominator = double(pow(rad, 2) * 2);

				val = factor * exp(-numerator / denominator);
			}
			
			return val;
		}
	};
}

#endif /* CONVOLUTIONIMAGEFILTER_H */

