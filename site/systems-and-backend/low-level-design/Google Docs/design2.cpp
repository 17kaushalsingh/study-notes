#include <iostream>
#include<fstream>

class DocumentElement {
public:
    virtual std::string render() = 0;
};

class TextElement : public DocumentElement {
    std::string text;

public:
    TextElement(std::string text) {
        this->text = text;
    }

    std::string render() override {
        return text + "\n";
    }
};

class ImageElement : public DocumentElement {
    std::string imagePath;

public:
    ImageElement(std::string imagePath) {
        this->imagePath = imagePath;
    }

    std::string render() override {
        std::string renderedImage = "[Imagefile: " + imagePath + "]" + "\n";
        return renderedImage;
    }
};

class TableElement : public DocumentElement {
    std::string tableData;

public:
    TableElement(std::string tableData) {
        this->tableData = tableData;
    }

    std::string render() override {
        std::string renderedTable = "[Tabledata: " + tableData + "]" + "\n";
        return tableData;
    }
};

class Document {
    std::vector<DocumentElement*> documentElements;
    std::string document;

    void render() {
        if (!document.empty()) {
            std::cout << "Document is already rendered\n";
            return;
        }

        for (auto &element : documentElements) {
            document += element->render();
        }
    }

public:
    void addElement(DocumentElement* element) {
        documentElements.push_back(element);
    }

    std::string getDocument() {
        if (document.empty()) render();
        return document;
    }
};

class Persistence {
public:
    virtual void save(std::string &data) = 0;
};

class DatabaseStorage : public Persistence {
public:
    void save(std::string &data) override {
        std::cout << "Saving document to database\n";
        std::cout << "Document saved to Database\n";
    }
};

class FileStorage : public Persistence {
    std::string fileName;

public:
    FileStorage(std::string fileName) {
        this->fileName = fileName;
    }

    void save(std::string &data) override{
        std::cout << "Saving document to file\n";

        std::ofstream file(fileName);
        if (file.is_open()) {
            file << data;
            file.close();
            std::cout << "Document saved to file\n";
        } else {
            std::cout << "Failed to save data to file at: " << fileName << "\n";
        }
        
    }
};

int main()
{
    Document* document = new Document();
    Persistence* file = new FileStorage("design2.txt");

    document->addElement(new TextElement("Hello, this is a sample design problem\nwhich I am trying to solve"));
    document->addElement(new ImageElement("image1.png"));
    document->addElement(new TableElement("Table x123hdf"));

    std::string data = document->getDocument();

    file->save(data);

    return EXIT_SUCCESS;
}