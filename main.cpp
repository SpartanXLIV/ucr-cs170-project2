#include <cmath> 
#include <vector>
#include <string> 
#include <iostream> 
#include <fstream> 
#include <sstream>
#include <algorithm>  
#include <list> 

using namespace std; 

int nearestNeighbor(vector<double> p, vector<vector<double>> t) 
{ 
    double minimum_Distance = 0.0; 
    int classType = p.at(0); 
    for(int i = 0; i < t.at(0).size(); i++) 
    { 
        double distance = 0.0; 
        for(int j = 1; j < t.size(); j++) 
        { 
			distance += pow(t.at(j).at(i) - p.at(j), 2); 
		} 
        if(i == 0)
        {
			minimum_Distance = distance;
		} 
        else if(distance < minimum_Distance)
        {
			minimum_Distance = distance; 
			classType = (int)t.at(0).at(i);
		}
    } 
    return classType; 
} 

double check_Valid(vector<vector<double>> set_features)
{ 
	vector<double> check_Instance; 
    vector <vector<double>> instanceSet; 
    
    int correct_counter = 0; 
    
    for(unsigned int i = 0; i < set_features.at(0).size(); i++)
    { 
        for(unsigned int j = 0; j < set_features.size(); j++)
        {
            vector<double> temp = set_features.at(j); 
            check_Instance.push_back(temp.at(i));
            temp.erase(temp.begin() + i);
            instanceSet.push_back(temp); 
        }
        
        if((int)nearestNeighbor(check_Instance, instanceSet) == (int)check_Instance.at(0)) 
        { 
			correct_counter++; 
		}
        instanceSet.clear(); 
    	check_Instance.clear();
    } 
    return (((double)correct_counter / (double)set_features.at(0).size()) * 100); 
} 

void forwardSelection(vector<vector<double>> instanceValues, int num_features)
{ 
	vector<int> best_Local; 
    vector<int> optimal_features; 
   
    double max = 0.0;
    double accuracy = 0.0;  
    
    for(int i = 1; i <= num_features; i++)
    { 
        double newMax = 0.0;  //to compare with max incase of local max
        vector<int> temp_Max; 
        for(int j = 1; j <= num_features; j++)
        {
            vector<vector<double>> temp; 
            vector<int> local_Temp; 
            temp.push_back(instanceValues.at(0)); 
            local_Temp = best_Local; 
            
            for(int x = 0; x < local_Temp.size(); x++)
            { 
				temp.push_back(instanceValues.at(local_Temp.at(x)));
			} 
                
            if(find(local_Temp.begin(), local_Temp.end(), j) == local_Temp.end()) 
            {
                temp.push_back(instanceValues.at(j)); 
                local_Temp.push_back(j); 
                cout << "Using feature(s) "; 
                for(int i = 0; i < local_Temp.size(); i++)
                {
					cout << local_Temp.at(i) << " "; 
				} 
                accuracy = check_Valid(temp); 
                cout << "accuracy is " << accuracy << "%" << endl; 
                if(accuracy > newMax)
                { 
					newMax = accuracy; 
					temp_Max = local_Temp; 
				}
            }   
        } 
        
        best_Local = temp_Max; 
        cout << "Feature set "; 
        
        for(int i = 0; i < best_Local.size(); i++)
        { 
			cout << best_Local.at(i) << " "; 
		} 
        cout << "was best, accuracy is " << newMax << "%" << endl << endl; 
        
        if(newMax > max) 
        { 
			optimal_features = best_Local; 
			max = newMax; 
			best_Local = optimal_features; 
		}
        else if(newMax < max)
        { 
			cout << "Accuracy is decreasing! Continuing search in case of local maxima..." << endl; 
		} 
    } 
    cout << endl << "Finished search!! The best feature subset is {"; 
    for(unsigned int i = 0; i < optimal_features.size(); i++)
    {
		if(i == optimal_features.size() - 1)
		{
			cout << optimal_features.at(i); 
		}
		else
		{
			cout << optimal_features.at(i) << ", "; 
		}
	} 
    cout << "}, which has an accuracy of " << max << "%" << endl; 
} 

void backward_Elimination(vector<vector<double>> instanceValues, int num_features) 
{ 
    vector<int> optimal_features(num_features); 	//stores the best features
    std::iota(optimal_features.begin(), optimal_features.end(), 1); 
    vector<int> best_Local = optimal_features; 
    
    double max = 0.0; 
    double accuracy = 0.0; 
    
    for(int i = 1; i < num_features; i++) 
    {
        double newMax = 0.0; 
        vector<int> temp_Max; 
        for(int j = 0; j <= num_features; j++)
        {
            vector<vector<double>> temp; 
            vector<int> local_Temp; 
            temp.push_back(instanceValues.at(0)); 
            local_Temp = best_Local; 
            
            for(int x = 0; x < best_Local.size(); x++) 
            { 
				temp.push_back(instanceValues.at(best_Local.at(x)));
			}
			 
            vector<int>::iterator it = find(local_Temp.begin(), local_Temp.end(), j);     
            if(it != local_Temp.end()) 
            {
                int index = it - local_Temp.begin() + 1; 
                local_Temp.erase(it);
                temp.erase(temp.begin() + index); 
                cout << "Using feature(s) "; 
                for(int i = 0; i < local_Temp.size(); i++)
                {
					cout << local_Temp.at(i) << " "; 
				} 
                accuracy = check_Valid(temp);  //error HERE not calling check_Valid
                cout << "accuracy is " << accuracy << "%" << endl; 
                if(accuracy >= newMax)
                { 
					newMax = accuracy;
					temp_Max = local_Temp; 
				} 
            }   
        } 
        best_Local = temp_Max; 
        cout << "Feature set "; 
        for(int i = 0; i < best_Local.size(); i++)
        { 
			cout << best_Local.at(i) << " "; 
		} 
        cout << "was best, accuracy is " << newMax << "%" << endl << endl; 
        
        if(newMax > max)
        {
			optimal_features = best_Local; 
			max = newMax;
			//best_Local = optimal_features;
		}
        else if(newMax < max)
        {
			cout << "Accuracy is decreasing! Continuing check in case of local maxima..." << endl; 
		} 
	} 
    cout << endl << "Finished search!! The best feature subset is {"; 
    for(int i = 0; i < optimal_features.size(); i++)
    {
		if(i == optimal_features.size() - 1)
		{
			cout << optimal_features.at(i); 
		}
		else
		{
			cout << optimal_features.at(i) << ", ";
		}
	} 
    cout << "}, which has an accuracy of " << max << "%" << endl; 
} 

int main() {
    vector< vector<double>> instanceValues;
    vector <double> set_features; 
    string txt_line; 
    
    string read_Feature; 
    
    int num_features;
    
    string filename, algType; 
    cout << "Welcome to Randy Zuniga Feature Selection Algorithm." << endl; 
    cout << "Type in the name of the file to test (CASE SENSITIVE): " << endl; 
    cin >> filename; 		//take user input to search for file type
    cout << "Type the number of the algorithm you want to run." << endl; 
    cout << "1) Forward Selection \n2) Backward Elimination" << endl; 
    cin >> algType; 
	
	ifstream fin; 					//read in the file name
    fin.open(filename.c_str());		//open the file
    if(!fin.is_open())				//if no such file or fail to open, throw an error
    {
        cout << "Error opening file." << endl; 
        return 0; 
    }
    
    getline(fin, txt_line); 
    std::stringstream sstream(txt_line); 

    while(sstream >> read_Feature)
    { 
        set_features.push_back(stold(read_Feature)); 
        instanceValues.push_back(set_features); 
        set_features.clear(); 
    } 

    while(getline(fin, txt_line)) 
    { 
        std::stringstream sstream(txt_line); 
        string read_Feature; 
        for(unsigned int i = 0; i < instanceValues.size(); i++) 
        { 
            if(sstream >> read_Feature) 
            { 
				instanceValues.at(i).push_back(stold(read_Feature)); 
			}  
        } 
    } 
    
	num_features = instanceValues.size() - 1; 
	
	cout << "This dataset has " << num_features << " features (not including the class attribute), "; 
	cout << "with " << instanceValues.at(0).size() << " instances." << endl; 
	cout << "Running nearest neighbor with all " << num_features << " features, using \"leaving-one-out\" evaluation, I get an accuracy of "; 
	cout << check_Valid(instanceValues) << "%" << endl; 
	cout << "Beginning search." << endl;
	
	if(algType == "2") 
	{ 
		 backward_Elimination(instanceValues, num_features); 
	}  
	else 
	{
		forwardSelection(instanceValues, num_features);  //default to algType == 1
	} 
	
	fin.close(); 	//close the file
	
    return 0; 
    
} 
