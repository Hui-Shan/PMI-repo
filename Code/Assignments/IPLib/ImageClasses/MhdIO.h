#include "ImageIOBase.h"
// Derived class MhdIO
class MhdIO : ImageIOBase
{
public:
	vector<short> read() override;
	void write(const array<int, N_DIM>&, const vector<short>&) override;
private:
	string get_relative_filepath();
};

// Functions for MhdIO
string MhdIO::get_relative_filepath()
	// Get relative folder path of filename if present
{
	string path_out;
	bool folder_in_filename = (filename.find(path_separator) != -1);
	if (folder_in_filename) {
		int path_idx = filename.find_last_of(path_separator) + 1;
		path_out = filename.substr(0, path_idx);
	}
	return path_out;
}

vector<short> MhdIO::read()
	// Reads in the image voxel values for the Mhd file named filename
{
	// Declare output vector with image data
	vector<short> image_vec;

	// and helper variables
	int ndims;
	vector<int> dimsize;

	string datafile;
	string elementtype;
	string file_path;
	string line;
	string param;
	string remainder;

	// Open ifstream for .mhd file
	ifstream ifs_mhd{ filename };
	if (!ifs_mhd) error("Could not open ", filename);

	// Get relative folder path of filename if present
	file_path = get_relative_filepath();

	// Read in the number of dimensions, the dimension sizes, the element type
	// and the name of the .raw file
	while (!ifs_mhd.eof()) {
		getline(ifs_mhd, line);
		if (line.size() > 0) {
			int equals_idx = line.find_first_of(" = ");
			param = line.substr(0, equals_idx);
			remainder = line.substr(equals_idx + 3);
			if (remainder.size() == 0) error("No value for parameter " + param + "\n");
			if (param == ndims_str) {
				ndims = stoi(line.substr(equals_idx + 3));
				cout << param << " " << ndims << "\n";
			}
			if (param == dimsize_str) {
				stringstream ss;
				ss << remainder;
				cout << param << " ";
				for (int dim; ss >> dim;) {
					if (dim < 0) error("Negative dimension found\n");
					dimsize.push_back(dim);
					cout << dim << " ";
				}
			}
			if (param == eltype_str) {
				elementtype = remainder;
				if (elementtype != MET_SHORT & elementtype != int16)
					error(filename + "has wrong data type\n");
			}
			if (param == eldatfile_str) {
				datafile = file_path + remainder;
			}
		}
	}

	// Open ifstream for the .raw file
	ifstream ifs_raw{ datafile, ios_base::binary };
	if (!ifs_raw) error("Could not open ", datafile);

	// and read in the image intensity values
	for (short val; ifs_raw.read(as_bytes(val), sizeof(short));) {
		image_vec.push_back(val);
	}

	// Throw error if the number of dimensions does not concur with the dimension
	// vector read
	if (ndims != dimsize.size()) error("Number of dimensions incorrect\n");

	// Throw error if the dimensions were not specified correctly to get the right
	// amount of voxels read
	int num_voxels_read = image_vec.size();
	int dim_product = 1;
	for (int dim : dimsize) {
		dim_product *= dim;
	}
	if (num_voxels_read != dim_product) error("Number of voxels incorrect\n");

	return image_vec;
}
