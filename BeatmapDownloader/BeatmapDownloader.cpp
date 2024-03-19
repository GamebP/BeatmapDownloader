#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <vector>
#include <algorithm>
#include <iomanip>

// Function to download a file from a URL using PowerShell
bool downloadFile(const std::string& url) {
    // Extract filename from URL
    size_t lastSlashIndex = url.find_last_of('/');
    if (lastSlashIndex == std::string::npos) {
        std::cerr << "Invalid URL format: " << url << std::endl;
        return false;
    }
    std::string filename = url.substr(lastSlashIndex + 1);

    // Construct the PowerShell command
    std::string command = "powershell -Command \"Invoke-WebRequest -Uri '" + url + "' -OutFile 'bMaps\\" + filename + ".osz'\"";

    // Execute the PowerShell command
    int result = std::system(command.c_str());

    return result == 0; // Check if the command was executed successfully
}

// Function to download files in a batch
void downloadBatch(int start_num, int num_requests) {
    std::vector<std::thread> threads;

    for (int i = start_num; i < start_num + num_requests; ++i) {
        std::string url = "https://osu.direct/api/d/" + std::to_string(i);
        threads.emplace_back([url]() {
            if (downloadFile(url)) {
                std::cout << "Downloaded successfully: " << url << std::endl;
            }
            else {
                std::cerr << "Failed to download: " << url << std::endl;
            }
            });
    }

    // Join all threads
    for (auto& thread : threads) {
        thread.join();
    }
}

int main() {
    // Prompt user for input
    int num_requests, start_num, batch_size;
    std::cout << "How much you want to download (example: 100): ";
    std::cin >> num_requests;
    std::cout << "From where you want to start? (example: 1000): ";
    std::cin >> start_num;
    std::cout << "Batch size (example: 10): ";
    std::cin >> batch_size;

    // Download files in batches
    for (int i = start_num; i < start_num + num_requests; i += batch_size) {
        int batch_requests = std::min(batch_size, num_requests - (i - start_num));
        downloadBatch(i, batch_requests);

        // Add a delay for demonstration purposes (optional)
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    return 0;
}
