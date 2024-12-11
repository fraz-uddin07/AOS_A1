#include<iostream>
#include<fcntl.h>
#include<sys/types.h>
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
            if (argc == 4) {
                return true;
            }
            else return false;
        }
};

class FrazUtil {
    public:

        static bool validate_directory (int directory_path, struct stat& directory_stat_buff) {
            if (fstat(directory_path, &directory_stat_buff) == 0 && S_ISDIR(directory_stat_buff.st_mode)) {
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


        static void print_permissions (int id, mode_t& file_mode) {
            if (id == 0) {
                cout << "User has read permissions on newfile : " << (file_mode & S_IRUSR ? "Yes" : "No") << endl;
                cout << "User has write permissions on newfile : " << (file_mode & S_IWUSR ? "Yes" : "No") << endl;
                cout << "User has execute permissions on newfile : " << (file_mode & S_IXUSR ? "Yes" : "No") << endl;

                cout << "Group has read permissions on newfile : " << (file_mode & S_IRGRP ? "Yes" : "No") << endl;
                cout << "Group has write permissions on newfile : " << (file_mode & S_IWGRP ? "Yes" : "No") << endl;
                cout << "Group has execute permissions on newfile : " << (file_mode & S_IXGRP ? "Yes" : "No") << endl;

                cout << "Others has read permissions on newfile : " << (file_mode & S_IROTH ? "Yes" : "No") << endl;
                cout << "Others has write permissions on newfile : " << (file_mode & S_IWOTH ? "Yes" : "No") << endl;
                cout << "Others has execute permissions on newfile : " << (file_mode & S_IXOTH ? "Yes" : "No") << endl;
            }
            else if (id == 1) {
                cout << "User has read permissions on oldfile : " << (file_mode & S_IRUSR ? "Yes" : "No") << endl;
                cout << "User has write permissions on oldfile : " << (file_mode & S_IWUSR ? "Yes" : "No") << endl;
                cout << "User has execute permissions on oldfile : " << (file_mode & S_IXUSR ? "Yes" : "No") << endl;

                cout << "Group has read permissions on oldfile : " << (file_mode & S_IRGRP ? "Yes" : "No") << endl;
                cout << "Group has write permissions on oldfile : " << (file_mode & S_IWGRP ? "Yes" : "No") << endl;
                cout << "Group has execute permissions on oldfile : " << (file_mode & S_IXGRP ? "Yes" : "No") << endl;

                cout << "Others has read permissions on oldfile : " << (file_mode & S_IROTH ? "Yes" : "No") << endl;
                cout << "Others has write permissions on oldfile : " << (file_mode & S_IWOTH ? "Yes" : "No") << endl;
                cout << "Others has execute permissions on oldfile : " << (file_mode & S_IXOTH ? "Yes" : "No") << endl;
            }
            else if (id == 2) {
                cout << "User has read permissions on directory : " << (file_mode & S_IRUSR ? "Yes" : "No") << endl;
                cout << "User has write permissions on directory : " << (file_mode & S_IWUSR ? "Yes" : "No") << endl;
                cout << "User has execute permissions on directory : " << (file_mode & S_IXUSR ? "Yes" : "No") << endl;

                cout << "Group has read permissions on directory : " << (file_mode & S_IRGRP ? "Yes" : "No") << endl;
                cout << "Group has write permissions on directory : " << (file_mode & S_IWGRP ? "Yes" : "No") << endl;
                cout << "Group has execute permissions on directory : " << (file_mode & S_IXGRP ? "Yes" : "No") << endl;

                cout << "Others has read permissions on directory : " << (file_mode & S_IROTH ? "Yes" : "No") << endl;
                cout << "Others has write permissions on directory: " << (file_mode & S_IWOTH ? "Yes" : "No") << endl;
                cout << "Others has execute permissions on directory : " << (file_mode & S_IXOTH ? "Yes" : "No") << endl;
            }
            // else //Invalid mode_t configuration
        }
};


void identical_file_check (int& new_file_descriptor, int& old_file_descriptor, char* new_file_buff, char* old_file_buff, uint64_t& total_bytes_read, uint64_t& total_bytes_written, const unsigned long long int& NEW_FILE_SIZE, const unsigned long long int& OLD_FILE_SIZE, const int& FINAL_BUFF_SIZE, bool& isFileContentReversed) {
    
    uint64_t iter_count = 1;
    uint64_t new_total_bytes_read = 0;
    uint64_t old_total_bytes_read = 0;
    uint64_t total_bytes_verified = 0;

    off_t new_curr_seek_idx = NEW_FILE_SIZE;
    off_t old_curr_seek_idx = 0;

    int percentage_written = 0, current_percent = 0;

    while (new_curr_seek_idx >= FINAL_BUFF_SIZE && old_curr_seek_idx <= (OLD_FILE_SIZE - FINAL_BUFF_SIZE)) {

        new_curr_seek_idx =  lseek(new_file_descriptor, -1 * (FINAL_BUFF_SIZE) * iter_count, SEEK_END);
        old_curr_seek_idx =  lseek(old_file_descriptor, (FINAL_BUFF_SIZE) * (iter_count - 1), SEEK_SET);
        ++iter_count;

        ssize_t new_bytes_read = read(new_file_descriptor, new_file_buff, FINAL_BUFF_SIZE);
        ssize_t old_bytes_read = read(old_file_descriptor, old_file_buff, FINAL_BUFF_SIZE);

        new_total_bytes_read += new_bytes_read;
        old_total_bytes_read += old_bytes_read;

        for (int i = 0, j = old_bytes_read - 1; i < new_bytes_read && j >= 0; i++, j--) {
            if (old_file_buff[j] != new_file_buff[i]) {
                isFileContentReversed = false;
                return;
            }
        }

        total_bytes_verified = new_total_bytes_read;

        current_percent = floor(((float)total_bytes_verified / (NEW_FILE_SIZE)) * 100);
        cout << "Verifying : " << current_percent << "%" <<  '\r';
    }

    //Handle Last Chunk
    off_t new_seek_idx_last_iter = lseek(new_file_descriptor, 0, SEEK_SET);
    off_t old_seek_idx_last_iter = lseek(old_file_descriptor, (FINAL_BUFF_SIZE) * (iter_count - 1), SEEK_SET);

    ssize_t new_bytes_read = read(new_file_descriptor, new_file_buff, new_curr_seek_idx - new_seek_idx_last_iter);
    ssize_t old_bytes_read = read(old_file_descriptor, old_file_buff, OLD_FILE_SIZE - old_seek_idx_last_iter);
    
    for (int i = 0, j = old_bytes_read - 1; i < new_bytes_read && j >= 0; i++, j--) {
        if (old_file_buff[j] != new_file_buff[i]) {
            isFileContentReversed = false;
            return;
        }
    }

    new_total_bytes_read += new_bytes_read;
    old_total_bytes_read += old_bytes_read;

    total_bytes_verified =  new_total_bytes_read;

    current_percent = floor(((float)total_bytes_verified / (NEW_FILE_SIZE)) * 100);
    cout << "Verifying : " << current_percent << "%" << '\r';

}


int main (int argc, char* argv[]) {

    if (!ErrorHandler::argument_validator(argc, argv)) {
        cout << "Bad Arguments" << endl;
        exit(EXIT_FAILURE);
    }

    const char* NEW_FILE_NAME = argv[1];      //Handle case where input file is wrong
    const char* OLD_FILE_NAME = argv[2];
    const char* DIRECTORY_PATH = argv[3];

    bool isDirectoryCreated = true;
    bool isFileContentReversed = true;
    bool isBothFileSizeSame = true;

    int new_file_descriptor = open(NEW_FILE_NAME, O_RDONLY);
    int old_file_descriptor = open(OLD_FILE_NAME, O_RDONLY);
    int directory_descriptor = open(DIRECTORY_PATH, O_RDONLY);

    struct stat new_file_stat_buff;
    struct stat old_file_stat_buff;
    struct stat directory_stat_buff;

    FrazUtil::validate_file(new_file_descriptor);
    FrazUtil::validate_file(old_file_descriptor);
    if (!FrazUtil::validate_directory(directory_descriptor, directory_stat_buff)) {
        isDirectoryCreated = false;
    }

    if (isDirectoryCreated) {
        cout << "Directory is created: Yes" << endl; 
    }
    else {
        cout << "Directory is created: No" << endl;
        exit(EXIT_FAILURE);
    }
    
    if (fstat(new_file_descriptor, &new_file_stat_buff) == -1 || fstat(old_file_descriptor, &old_file_stat_buff) == -1 ) {
        cout << "Error in creating file_statistics buffer";
        exit(EXIT_FAILURE);
    }

    const unsigned long long int NEW_FILE_SIZE = new_file_stat_buff.st_size;
    const unsigned long long int OLD_FILE_SIZE = old_file_stat_buff.st_size;
    const unsigned long long int OPTIMUM_BLK_SIZE = min(new_file_stat_buff.st_blksize, old_file_stat_buff.st_blksize);  //May cause problems - behaviour unknown
    
    // cout << "New File Size " << NEW_FILE_SIZE << "Old File Size "  << OLD_FILE_SIZE << " Optimal_buff_size " << OPTIMUM_BLK_SIZE << endl;
        const int FINAL_BUFF_SIZE = min(min(OPTIMUM_BLK_SIZE, NEW_FILE_SIZE), OLD_FILE_SIZE);
    // cout << "FINAL_BUFF_SIZE " << FINAL_BUFF_SIZE << endl;

    char new_file_buff[FINAL_BUFF_SIZE];
    char old_file_buff[FINAL_BUFF_SIZE];

    uint64_t new_total_bytes_read = 0;
    uint64_t old_total_bytes_read = 0;

    if (NEW_FILE_SIZE != OLD_FILE_SIZE) {
        isBothFileSizeSame = false;
        isFileContentReversed = false;
    }

    if (isFileContentReversed) {
        identical_file_check(new_file_descriptor, old_file_descriptor, new_file_buff, old_file_buff, new_total_bytes_read, old_total_bytes_read, NEW_FILE_SIZE, OLD_FILE_SIZE, FINAL_BUFF_SIZE, isFileContentReversed);
    }

    if (isFileContentReversed) {
        cout << "Whether file contents are reversed in newfile: Yes" << endl;
    }
    else {
        cout << "Whether file contents are reversed in newfile: No" << endl;
    }

    if (isBothFileSizeSame) {
        cout << "Both File Sizes are Same: Yes" << endl;
    }
    else {
        cout << "Both File Sizes are Same: No" << endl;
    }
    cout << endl;


    //Check Permissions
    mode_t new_file_mode = new_file_stat_buff.st_mode;
    mode_t old_file_mode = old_file_stat_buff.st_mode;
    mode_t directory_mode = directory_stat_buff.st_mode;

    //Print Permissions
    // 0 for New File, 1 for old_file, 2 for Directory
    FrazUtil::print_permissions(0, new_file_mode);
    FrazUtil::print_permissions(1, old_file_mode);
    FrazUtil::print_permissions(2, directory_mode);

    cout << endl;
    
    close(new_file_descriptor);
    close(old_file_descriptor);

    return 0;
}