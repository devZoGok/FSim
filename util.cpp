#include"util.h"

using namespace std;
using namespace chrono;
using namespace boost::filesystem;

namespace fsim{
	bool isSuchFile(boost::filesystem::path path){
		bool found=false,foundFileName=false;
            string fileName;
            boost::filesystem::path mapDir;
            int length=path.string().length();
            for(int i=length;i>=0&&!foundFileName;i--)
                if(path.string().c_str()[i]=='/'){
                    fileName=path.string().substr(i+1,length-i);
                    mapDir=path.string().substr(0,i);
                    foundFileName=true;
                }
            directory_iterator dir;
            for(directory_iterator itr(mapDir);itr!=dir;itr++){
                if(itr->path().string()==path.string())
                    found=true;
            }
            return found;
        }

        void getRecDirs(path p,std::vector<string> &list,bool showFiles, bool showDirs, bool rec){
            directory_iterator dir;
            for(directory_iterator itr(p);itr!=dir;itr++){
                if((showFiles&&is_regular_file(itr->path()))||(showDirs&&is_directory(itr->path())))
                    list.push_back((string)itr->path().c_str());
                    if(rec&&is_directory(itr->path()))
                        getRecDirs(itr->path(),list,showFiles,showDirs,rec);
            }
        }
        
        void readFile(string path,std::vector<string> &lines,int firstLine,int lastLine) {
            std::ifstream inFile(path);
            if(lastLine!=-1)
                for(int i=0;i<lastLine;i++){
                    string l;
                    inFile>>l;
                    if(i>=firstLine)
                        lines.push_back(l);
                }
            else{
                string l;
                while(getline(inFile,l))
                    lines.push_back(l);
            }
            inFile.close();
        }

		void writeFile(string path,std::vector<string> &lines) {
			std::ofstream outFile(path);
		   	for(string l : lines)
			   	outFile<<l<<"\n";
			outFile.close();
		}
}
