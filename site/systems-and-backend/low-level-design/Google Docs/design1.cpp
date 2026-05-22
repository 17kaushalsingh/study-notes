#include <iostream>
#include<fstream>

class DocumentEditor {
    std::vector<std::string> documentElements;
    std::string document;

    bool isImagePath(std::string &path) {
        // Allowed image paths: .png .jpg

        int pathLen = path.size();

        if (pathLen >= 4) {
            if (path.substr(pathLen - 4, 4) == ".png")
                return true;
            if (path.substr(pathLen - 4, 4) == ".jpg")
                return true;
        }

        return false;
    }

public:
    void addText(std::string &text) {
        documentElements.push_back(text);
    };

    void addImage(std::string &imagePath) {
        documentElements.push_back(imagePath);
    };

    void renderDocument() {
        if (!document.empty()) {
            std::cout << "Document is already rendered\n";
            return ;
        }

        for (std::string &str : documentElements) {
            if (isImagePath(str)) {
                document += "Image added: " + str + "\n";
            }
            else {
                document += str + "\n";
            }
        };
    }

    void printDocument() {
        std::cout << "###############\n";
        std::cout << document;
        std::cout << "###############\n";
    }

    void saveToFile(std::string &fileName) {
        std::ofstream file(fileName);

        if (file.is_open()) {
            if (document.empty()) renderDocument();
            file << document;
            file.close();
            std::cout << "Document saved to file: " << fileName << "\n";
        } else {
            std::cout << "Failed to locate file at: " << fileName << "\n";
        }
    }
};

int main()
{
    DocumentEditor *editor = new DocumentEditor();

    std::string image1 = "images/image1.png";
    std::string text1 = "Image description: this is a sample image";
    editor->addImage(image1);
    editor->addText(text1);

    editor->renderDocument();
    // editor->printDocument();

    std::string fileName = "design1.txt";
    editor->saveToFile(fileName);

    return EXIT_SUCCESS;
}