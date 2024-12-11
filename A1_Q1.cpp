#include<iostream>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>
#include<math.h>
#include<cstring>
#include<chrono>

using namespace std;

class Logger {
    public:
        static void log(string& msg) {
            cout << msg << endl;
        }
};

class ErrorHandler {
    public:

        static bool argument_validator(int& argc, char* argv[]) {
            if (argc == 3) {
                if (strlen(argv[2]) == 1 && argv[2][0] == '0') {
                    return true;
                }
                else return false;
            }
            else if (argc == 5) {
                if (strlen(argv[2]) == 1 && argv[2][0] == '1') {
                    return true;
                }
                else return false;
            }
            else return false;
        }
};

class FrazUtil {
    public:

        static char* create_output_file_name (char* flag, const char* input_filename) {
            int output_filename_size = strlen(flag) + strlen(input_filename) + 1 + 12;
            char* output_filename = (char*)malloc(output_filename_size * sizeof(char));
            char prefix[] = "Assignment1/";
            int prefix_len = strlen(prefix);

            strcpy(output_filename, prefix);

            output_filename[prefix_len] = flag[0];
            output_filename[prefix_len + 1] = '_';
            for (int i = prefix_len + 2; i < prefix_len + 2 + strlen(input_filename); i++) {
                output_filename[i] = input_filename[i - 14];
            }
            return output_filename;
        }


        static string extract_input_file_name (char* input_file_path) {
            string file_path = input_file_path;
            int i;
            for (i = file_path.size() - 1; i >= 0; i--) {
                if (file_path[i] == '/') {
                    break;
                }
            }
            string input_file_name = file_path.substr(i + 1, file_path.size() - 1 - i);

            return input_file_name;
        }


        static inline bool is_index_valid (uint64_t& index_of_start, uint64_t& index_of_end, const unsigned long long int& INPUT_FILE_SIZE) {
            if (index_of_start >= 0 && index_of_start < INPUT_FILE_SIZE && index_of_end >= 0 && index_of_end < INPUT_FILE_SIZE && index_of_start <= index_of_end) {
                return true;
            }
            else return false;
        }


        static void validate_file (int& fd) {
            if (fd < 0) {
                cout << "Problem Opening File" << endl;
                exit(EXIT_FAILURE);
            }
        }


        bool is_file_invalid () {
            return false;
        }
};


void file_copy (int& input_file_descriptor, int& output_file_descriptor, char* input_buff, char* output_buff, uint64_t& index_of_start, uint64_t& index_of_end, uint64_t& total_bytes_read, uint64_t& total_bytes_written, const unsigned long long int& INPUT_FILE_SIZE, const int& FINAL_BUFF_SIZE) {
    uint64_t iter_count = 0;
    off_t curr_seek_idx = index_of_start;
    int percentage_written = 0, current_percent = 0;

    while (index_of_end - curr_seek_idx + 1 >= FINAL_BUFF_SIZE) {

        curr_seek_idx =  lseek(input_file_descriptor, index_of_start + iter_count * FINAL_BUFF_SIZE, SEEK_SET);
        ++iter_count;

        ssize_t bytes_read = read(input_file_descriptor, input_buff, FINAL_BUFF_SIZE);
        curr_seek_idx += bytes_read;
        total_bytes_read += bytes_read;

        ssize_t bytes_written = write(output_file_descriptor, input_buff, FINAL_BUFF_SIZE);
        total_bytes_written += bytes_written;

        current_percent = floor(((float)total_bytes_written / (INPUT_FILE_SIZE)) * 100);
        if (current_percent > percentage_written) {
            percentage_written = current_percent;
        } 
        cout << percentage_written << "%" << '\r';
    }   

    //Handle Last Chunk
    off_t seek_idx_last_iter = lseek(input_file_descriptor, curr_seek_idx, SEEK_SET);

    ssize_t bytes_read = read(input_file_descriptor, input_buff, index_of_end - seek_idx_last_iter + 1);
    
    total_bytes_read += bytes_read;

    ssize_t bytes_written = write(output_file_descriptor, input_buff, bytes_read);
    
    total_bytes_written += bytes_written;

    current_percent = floor(((float)total_bytes_written / (INPUT_FILE_SIZE)) * 100);
    percentage_written = current_percent;
    cout << percentage_written << "%" << '\r';

}


void file_reverse (int& input_file_descriptor, int& output_file_descriptor, char* input_buff, char* output_buff, uint64_t& index_of_start, uint64_t& index_of_end, uint64_t& total_bytes_read, uint64_t& total_bytes_written, const unsigned long long int& INPUT_FILE_SIZE, const int& FINAL_BUFF_SIZE) {
    uint64_t iter_count = 1;
    off_t curr_seek_idx = index_of_end + 1;
    int percentage_written = 0, current_percent = 0;

    while (curr_seek_idx - index_of_start >= FINAL_BUFF_SIZE) {

        curr_seek_idx =  lseek(input_file_descriptor, index_of_end - (FINAL_BUFF_SIZE) * iter_count + 1, SEEK_SET);
        ++iter_count;

        ssize_t bytes_read = read(input_file_descriptor, input_buff, FINAL_BUFF_SIZE);

        total_bytes_read += bytes_read;

        for (int i = 0, j = bytes_read - 1; i < bytes_read; i++, j--) {
            output_buff[i] = input_buff[j];
        }

        ssize_t bytes_written = write(output_file_descriptor, output_buff, FINAL_BUFF_SIZE);

        total_bytes_written += bytes_written;

        current_percent = floor(((float)total_bytes_written / (INPUT_FILE_SIZE)) * 100);
        percentage_written = current_percent;
        cout << percentage_written << "%" <<  '\r';
    }

    //Handle Last Chunk
    off_t seek_idx_last_iter = lseek(input_file_descriptor, index_of_start, SEEK_SET);

    ssize_t bytes_read = read(input_file_descriptor, input_buff, curr_seek_idx - seek_idx_last_iter);
    
    total_bytes_read += bytes_read;
    for (int i = 0, j = bytes_read - 1; i < bytes_read; i++, j--) {
            output_buff[i] = input_buff[j];
    }

    ssize_t bytes_written = write(output_file_descriptor, output_buff, bytes_read);

    total_bytes_written += bytes_written;

    current_percent = floor(((float)total_bytes_written / (INPUT_FILE_SIZE)) * 100);
        percentage_written = current_percent;
    cout << percentage_written << "%" << '\r';

}


int main (int argc, char* argv[]) {

    if (!ErrorHandler::argument_validator(argc, argv)) {
        cout << "Bad Arguments" << endl;
        exit(EXIT_FAILURE);
    }

    bool flag = stoi(argv[2]);

    string INPUT_FILE_NAME = FrazUtil::extract_input_file_name(argv[1]);      //Handle case where input file is wrong

    const char* OUTPUT_FILE_NAME = FrazUtil::create_output_file_name(argv[2], INPUT_FILE_NAME.c_str());

    int input_file_descriptor = open(argv[1], O_RDONLY);
    FrazUtil::validate_file(input_file_descriptor);

    if (mkdir("Assignment1/", S_IRWXU) < -1) {
        cout << "Unable to create Directory" << endl;
        cout << "Error number " << errno << endl;
        exit(EXIT_FAILURE);
    }

    struct stat input_file_stat_buff;

    if (fstat(input_file_descriptor, &input_file_stat_buff) == -1) {
        cout << "Error in creating file_statistics buffer";
        exit(EXIT_FAILURE);
    }
    const unsigned long long int INPUT_FILE_SIZE = input_file_stat_buff.st_size;       //Includes the null character
    const unsigned long long int OPTIMUM_BLK_SIZE = input_file_stat_buff.st_blksize;  //May cause problems - behaviour unknown
    
    int output_file_descriptor = creat(OUTPUT_FILE_NAME, O_RDWR | O_CREAT | S_IRUSR | S_IWUSR);     //Delete directory if it exists ****[4]

    // cout << "File Size " << INPUT_FILE_SIZE << " Optimal_buff_size " << OPTIMUM_BLK_SIZE << endl;

    // const int FINAL_BUFF_SIZE = min(optimum_blk_size, input_file_size - 1);       //Change for large files
        const int FINAL_BUFF_SIZE = min(OPTIMUM_BLK_SIZE, INPUT_FILE_SIZE);
    // cout << "FINAL_BUFF_SIZE " << FINAL_BUFF_SIZE << endl;

    char input_buff[FINAL_BUFF_SIZE];
    char output_buff[FINAL_BUFF_SIZE];
    const char NULL_BUFF[1] = {'\0'};

    uint64_t total_bytes_read = 0;
    uint64_t total_bytes_written = 0;

    if (!flag) {
        //Reverse the content of the file in range [idx_of_start, idx_of_end]
        uint64_t start_idx = 0;
        uint64_t end_idx = INPUT_FILE_SIZE - 1;
        file_reverse(input_file_descriptor, output_file_descriptor, input_buff, output_buff, start_idx, end_idx, total_bytes_read, total_bytes_written, INPUT_FILE_SIZE, FINAL_BUFF_SIZE);
    }
    else {
        uint64_t index_of_start = stoi(argv[3]), index_of_end = stoi(argv[4]);
        if (!FrazUtil::is_index_valid(index_of_start, index_of_end, INPUT_FILE_SIZE)) {
            cout << "Invalid Index" << endl;
            exit(EXIT_FAILURE);
        }
        //Reverse the content of the file in range [idx_of_start, idx_of_end]
        uint64_t start_idx = 0, end_idx = index_of_start - 1;
        file_reverse(input_file_descriptor, output_file_descriptor, input_buff, output_buff, start_idx, end_idx, total_bytes_read, total_bytes_written, INPUT_FILE_SIZE, FINAL_BUFF_SIZE);
        
        //Copy the contents of the file in range [idx_of_start, idx_of_end]
        start_idx = index_of_start, end_idx = index_of_end; 
        file_copy(input_file_descriptor, output_file_descriptor, input_buff, output_buff, start_idx, end_idx, total_bytes_read, total_bytes_written, INPUT_FILE_SIZE, FINAL_BUFF_SIZE);
    
        //Reverse the content of the file in range [idx_of_start, idx_of_end]
        start_idx = index_of_end + 1, end_idx = INPUT_FILE_SIZE - 1; 
        file_reverse(input_file_descriptor, output_file_descriptor, input_buff, output_buff, start_idx, end_idx, total_bytes_read, total_bytes_written, INPUT_FILE_SIZE, FINAL_BUFF_SIZE);
    
    }

    cout << endl;
    
    close(input_file_descriptor);
    close(output_file_descriptor);

    return 0;
}