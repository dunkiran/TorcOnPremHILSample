// #include <aws/s3/S3Client.h>
// #include <aws/core/Aws.h>
// #include <aws/core/auth/AWSCredentialsProvider.h>
// #include <aws/transfer/TransferManager.h>
// #include <aws/transfer/TransferHandle.h>
#include <iostream>
#include <fstream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */


int fibonacci(int& prev, int& prev2) {
    int result = prev + prev2;
    prev2 = prev;
    prev = result;
    return result;
}

/*
 *  usage: ./s3_test srcFile bucketName destFile region
 *  this function is using tranfer manager to copy a local file to the bucket
 */
int main(int argc, char *argv[])
{
    if(argc != 5){
        std::cout << "usage: ./s3_test srcFile bucketName destFile region\n";
        return 1;
    }
    std::string fileName = argv[1]; //local FileName to be uploaded to s3 bucket
    std::string bucketName = argv[2];  //bucketName, make sure that bucketName exists
    std::string objectName = argv[3];
    std::string region = argv[4];

    
    std::cout << "file " << fileName << " will not be uploaded to bucket " << bucketName << " as " << objectName << " in " << region << std::endl;

    // initialize the random seed
    srand(time(NULL));

    int prev2 = rand() % 10;
    int prev = (rand() % 9) + 1;

    if (prev < prev2) {
        int swap = prev;
        prev = prev2;
        prev2 = swap;
    }

    std::cout << "\t0: \t" << prev2 << std::endl;
    std::cout << "\t1: \t" << prev << std::endl;

    for (int i = 2; i < 10; i++) {
        std::cout << "\t" << i << ": \t" << fibonacci(prev, prev2) << std::endl;
    }

    std::cout << "Done" << std::endl;
    return 0;
}
