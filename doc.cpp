#include<iostream>
#include<vector>
#include <string>
#include <fstream>

using namespace std;

//abstraction for document elements
class DocumentElement{
    public:
    virtual string render() = 0;
};

//concrete implemention for text elements
class TextElement :public DocumentElement{
    private:
    string text;

    public:
    TextElement(string text){
        this->text = text;
    }

    string render() override{
        return text;
    }
};

//concrete implemention for image elements
class ImageElement: public DocumentElement{
    private: 
    string imagePath;

    public:
    ImageElement(string imagePath){
        this->imagePath = imagePath;
    }

    string render() override{
        return "[Image: " + imagePath + "]";
    }
};

//reprent a line break in the document.
class NewLineElement : public DocumentElement{
    public:
    string render() override{
        return "\n";
    }
};

//reprent a tab space in the document.
class TabSpaceElemnt: public DocumentElement{
    public:
    string render() override{
        return "\t";
    }
};

//class responsible for holding a collection of elements
class Document{
    private:
    vector<DocumentElement*> documentElements;

    public:
    void addElement(DocumentElement* element){
        documentElements.push_back(element);
    }

    //Render the document by concatenating the render output of all elements.
    string render(){
        string result;
        for(auto element: documentElements){
            result += element->render();
        }
        return result;
    }
};

//persistance Interface 
class Persistance{
    public:
    virtual void save(string data) = 0;
};

//fileStorage implemention of Persistance
class FileStorage : public Persistance{
    public:
    void save(string data) override{
        ofstream outFile("document.txt");
        if(outFile){
            outFile << data;
            outFile.close();
           cout<<"Document saved to document.txt"<<endl;
        }else{
            cout<<"Error: Unable to open file for writiing"<<endl;
        }
    }
};

class DBStorage : public Persistance{
    public:
    void save(string data) override{
        //we can save our data in db here
    }
};

//this class manages the interaction with client
class DocumentEditor{
    private:
    Document* document;
    Persistance* storage;
    string renderedDocument;

    public:

    DocumentEditor(Document* document, Persistance* storage){
        this->document = document;
        this->storage = storage;
    }

    void addText(string text){
        document->addElement(new TextElement(text));
    }

    void addImage(string imagePath){
        document->addElement(new ImageElement(imagePath));
    }

    void addNewLine(){
        document->addElement(new NewLineElement());
    }

    void addTabSpace(){
        document->addElement(new TabSpaceElemnt());
    }
    string renderDocument(){
        if(renderedDocument.empty()){
            renderedDocument = document->render();
        }
        return renderedDocument;
    }

    void saveDocument(){
        storage->save(renderDocument());
    }
};

int main(){
   Document* document = new Document();
   Persistance* persistance = new FileStorage();

   DocumentEditor* editor = new DocumentEditor(document, persistance);

   //Simulate a client using the editor with common text formatting features.
   editor->addText("Hello, world!");
   editor->addNewLine();
   editor->addText("This is a real-world document editor example.");
   editor->addNewLine();
   editor->addTabSpace();
   editor->addText("Indeted text after a tab space.");
   editor->addNewLine();
   editor->addImage("picture.jpg");

   //Render and display the final document.
   cout << editor->renderDocument()<<endl;

   editor->saveDocument();
    return 0;
}