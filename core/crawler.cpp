/* download to file name
check domain as substring
fix bug in allowed
test download
check max page count
make crawl function to eliminate main
*/

#include <iostream>
#include <urlmon.h>
#include <vector>
#include <fstream>
#pragma comment(lib, "urlmon.lib")
using namespace std;

class Crawler
{
public:
	char * startUrl;
	vector<char*> queue;
	vector<char*> doneQueue;
	vector<char*> content;
	vector<char*> blacklistedDomains;
	vector<char*> allowedDomains;
	double maxPageCount;
	// getting and setting functions
	double getPageCount();
	void setPageCount(double d);
	char* getStartUrl();
	void setStartUrl(char * c);
	void addBlacklist(char * c);
	void addAllowed(char * c);
	vector<char*>& getBlacklisted();
	vector<char*>& getAllowed();
	// crawling functions
	void download(char * c);
	void check(char * c);
	void crawl(char * start, vector<char*> blacklist, vector<char*> allowed, double max);
	bool notBlacklisted(char * c);
	bool allowed(char * c);
};
double Crawler::getPageCount()
{
	return maxPageCount;
}
void Crawler::setPageCount(double d)
{
	maxPageCount = d;
}
char* Crawler::getStartUrl()
{
	return startUrl;
}
void Crawler::setStartUrl(char * c)
{
	int length = 0;
	while(c[length] != '\0')
	{
		length++;
	}
	startUrl = new char[length+1];
	for(int i = 0; i < (length + 1);i++)
	{
		startUrl[i] = c[i];
	}
}
void Crawler::download(char * webAddress)
{
/*
	int length = 0;
	while(webAddress[length] != '\0')
	{
		length++;
	}
	char szFileName[length+1];
	for(int i = 0; i < (length + 1);i++)
	{
		szFileName[i] = webAddress[i];
	}
	HRESULT hr = URLDownloadToFile(NULL,webAddress,szFileName,0,NULL);
	if(hr != S_OK)
	{
		cout << "Operation failed with error code:" << hr << "\n";
	}
*/
}
void Crawler::addBlacklist(char * c)
{
	char * newBlacklisted;
	int i = 0;
	while(c[i] != '\0')
	{
		i++;
	}
	newBlacklisted = new char[i+1];
	for(int s = 0; s < (i+1); s++)
	{
		newBlacklisted[s] = c[s];
	}
	newBlacklisted[i+1] = '\0';
	blacklistedDomains.push_back(newBlacklisted);
}
void Crawler::addAllowed(char * c)
{
	char * newAllowed;
	int i = 0;
	while(c[i] != '\0')
	{
		i++;
	}
	newAllowed = new char[i+1];
	for(int s = 0; s < (i+1); s++)
	{
		newAllowed[s] = c[s];
	}
	newAllowed[i+1] = '\0';
	allowedDomains.push_back(newAllowed);
}
vector<char*>& Crawler::getBlacklisted()
{
	return blacklistedDomains;
}
vector<char*>& Crawler::getAllowed()
{
	return allowedDomains;
}
bool Crawler::notBlacklisted(char * c)
{
	if(blacklistedDomains.size() > 0)
		for(int i = 0; i < blacklistedDomains.size(); i++)
		{
			if(!strcmp(c,blacklistedDomains[i]))
				return false;
		}
	return true;
}
bool Crawler::allowed(char * c)
{
	string arg = c;
	if(allowedDomains.size() > 0)
	{	
		for(int i = 0; i < allowedDomains.size(); i++)
		{
			string current = allowedDomains[i];
			if(arg.find(current) != string::npos)
				return true;
		}
		return false;
	}
	else
		return true;
}
void Crawler::check(char * fileName)
{
	fstream f(fileName, fstream::in );
  	string content;
  	getline( f, content, '\0');
	f.close();
	string search = "http";
	int contentLocation = 0;
	while(true)
	{
		boolean duplicate = false;
		int startpos = content.find(search,contentLocation);
		if(startpos <= 0)
			break;
		int endpos = content.find("\"",startpos);
		string url = content.substr(startpos, (endpos-startpos));
		contentLocation = endpos;
		for(int s = 0; s < queue.size(); s++)
		{
			if(queue[s] == (url))
			{
				duplicate = true;
				break;
			}
		}
		if(!duplicate)
		{
			for(int b = 0; b < doneQueue.size(); b++)
			{
				if(queue.at(b) == url)
				{
					duplicate = true;
					break;
				}
			}
		}
		if(!duplicate)
		{
			char * URL;
			int length = url.size();
			URL = new char[length];
			for(int i = 0; i < length; i++)
				URL[i] = url[i];
			URL[length] = '\0';
			if(notBlacklisted(URL) && allowed(URL))
				queue.push_back(URL);
		}	
	}	
}
void Crawler::crawl(char * start, vector<char*> blacklist, vector<char*> allowed, double max = 1000)
{
	setStartUrl(start);
	// set blacklist and allowed
	maxPageCount = max;
	download(startUrl);
	doneQueue.push_back(startUrl);
	queue.erase(queue.begin(),queue.begin()+1);
	double pageCount = 1;
	while(queue.size() != 0 && pageCount <= maxPageCount)
	{
		download(queue[0]);
		check(queue[0]);
		doneQueue.push_back(queue[0]);
		queue.erase(queue.begin(),queue.begin()+1);
	}
}
int main()
{
	Crawler sharp;
	sharp.addAllowed("");
	sharp.check("tester.txt");
	for(int i = 0; i < sharp.queue.size(); i++)
	{
		cout << sharp.queue[i] << endl;
	}
}