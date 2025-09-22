#include <bits/stdc++.h>
using namespace std;

static inline string trim(const string &x) {
    size_t a = 0, b = x.size();
    
	while (a < b && isspace((unsigned char)x[a])) ++a;
    while (b > a && isspace((unsigned char)x[b - 1])) --b;
    
	return x.substr(a, b - a);
}

static inline vector<string> distinguish(const string &x, char Q){
    
	vector<string> components; string compo;
    for (size_t i = 0; i < x.size(); ++i){
        
		char c = x[i];
        if (c == Q){ components.push_back(compo); compo.clear(); }
        else compo.push_back(c);
    }
    
	components.push_back(compo);
    return components;
}

static inline string repeat(double y){
    
	ostringstream oss; oss.setf(ios::fixed); oss << setprecision(12) << y;
    string z = oss.str();
    while(!z.empty() && z.back() == '0') z.pop_back();
    
	if (!z.empty() && z.back() == '.') z.pop_back();
    if (z.empty()) z = "0";
    
	return z;
}

static inline bool swith(const string& d, const string& e){
    return d.rfind(e, 0) == 0;
}

struct student{
    
	int STU_ID = 0;
    string Name;
    string Surname;
    string National_ID;
    double GPA = 0.0;
};

struct GPA_STU{
    
	bool operator()(const student *i, const student *j)const{
        if(i -> GPA != j -> GPA) return i -> GPA > j -> GPA;
        
		return i -> STU_ID < j -> STU_ID;
    }
};

class DBase{
    
	const int MIN_SID = 1000001;
    map<int, student> tree;
    map<string, int> NID;
    
	priority_queue<int, vector<int>, greater<int> > idle_ID;
    int Next = MIN_SID;

public:
    void loading(const string &way){
        
		ifstream within(way); if(!within) return;
        string line; vector<int> once;
        while(getline(within, line)){
            
			if(line.empty()) continue;
            vector<string> components = distinguish(line, '|');
            if(components.size() != 5) continue;
            
			student x;
            x.STU_ID      = atoi(components[0].c_str());
            x.Name        = components[1];
            x.Surname     = components[2];
            x.National_ID = components[3];
            x.GPA         = atof(components[4].c_str());
            
			tree[x.STU_ID] = x;
            NID[x.National_ID] = x.STU_ID;
            once.push_back(x.STU_ID);
        }
        sort(once.begin(), once.end());
        
		int compo = MIN_SID;
        for (size_t f = 0; f < once.size(); ++f){
            int STID = once[f];
            while (compo < STID){ idle_ID.push(compo); ++compo; }
            compo = STID + 1;
        }
        
		Next = compo < MIN_SID ? MIN_SID : compo;
    }

    void save(const string &way){
        ofstream outside(way, ios::trunc);
        
		for (map<int, student>::const_iterator me = tree.begin(); me != tree.end(); ++me){
            const student &x = me -> second;
            outside << x.STU_ID << '|' << x.Name << '|' << x.Surname << '|' << x.National_ID << '|' << repeat(x.GPA) << "\n";
        }
    }

    bool empty() const { return tree.empty(); }

    int distribute() {
        
		if(!idle_ID.empty()){
            
			int y = idle_ID.top(); idle_ID.pop();
            return y;
        }
        
		int y = Next < MIN_SID ? MIN_SID : Next;
        Next = y + 1;
        
		return y;
    }

    void again(int STID){
        if (STID >= MIN_SID) idle_ID.push(STID);
    }

    bool srch(int STID){
        
		map<int, student>::iterator him = tree.find(STID);
        if (him == tree.end()){
            
			cout << "NOT FOUND\n";
            return false;
        }
        
		const student &x = him -> second;
        show(x);
        
		return true;
    }

    bool rank_GPA(){
        if(tree.empty()){ cout << "FAILED => NOT FOUND\n"; return false; }
        
		const student *high = 0;
        for (map<int, student>::const_iterator me = tree.begin(); me != tree.end(); ++me){
            
			const student &x = me -> second;
            if (!high || x.GPA > high -> GPA) high = &x;
        }
        
        double best = high -> GPA;
    	vector<const student*> ties;
    	
		for (map < int, student>::const_iterator me = tree.begin(); me != tree.end(); ++me){
        
			if (me -> second.GPA == best) ties.push_back(&me -> second);
        
		}
		
		sort(ties.begin(), ties.end(),
        	
			[](const student* a, const student* b){ return a -> STU_ID < b -> STU_ID; });
        
        for (size_t i = 0; i < ties.size(); ++i){
        show(*ties[i]);
        
		if (i + 1 < ties.size()) cout << "---------------------\n";
        
		}
        return true;
    }

    bool Avg_STU(double y){
        
		if(tree.empty()){ cout << "FAILED => NOT FOUND\n"; return false; }
        vector<const student*> twice;
        
		for (map<int, student>::const_iterator me = tree.begin(); me != tree.end(); ++me)
            if (me -> second.GPA >= y) twice.push_back(&me -> second);
        
		if (twice.empty()){ cout << "FAILED => NOT FOUND\n"; return false; }
        sort(twice.begin(), twice.end(), GPA_STU());
        for (size_t a = 0; a < twice.size(); ++a){
            
			show(*twice[a]);
            if (a + 1 < twice.size()) cout << "---------------------\n";
        }
        return true;
    }

    void submit_STU (const string &Name, const string &Surname, const string &National_ID, double GPA){
        if (NID.count(National_ID)){
            cout << "FAILED => RECORD EXISTS\n"; return;
        }
        
		int STID = distribute();
        student x;
        x.STU_ID      = STID;
        x.Name        = Name;
        x.Surname     = Surname;
        x.National_ID = National_ID;
        x.GPA         = GPA;
        
		tree[STID] = x;
        NID[National_ID] = STID;
        cout << "SUCCESS => STU_ID:" << STID << "\n";
    }

    void remove(int STID){
        if (tree.empty()){ cout << "FAILED => NOT FOUND\n"; return; }
        
		map<int, student>::iterator him = tree.find(STID);
        if (him == tree.end()){ cout << "FAILED => NOT FOUND\n"; return; }
        
		again(STID);
        NID.erase(him -> second.National_ID);
        tree.erase(him);
        
		cout << "SUCCESS\n";
    }

    void below_GPA(){
    if (tree.empty()){ cout << "FAILED => NOT FOUND\n"; return; }

    vector<student> removing;
    for (map<int, student>::const_iterator me = tree.begin(); me != tree.end(); ++me)
        
		if(me -> second.GPA < 12.0) removing.push_back(me -> second);

    if (removing.empty()){ cout << "FAILED => NOT FOUND\n"; return; }

    for (size_t i = 0; i < removing.size(); ++i){
        const student &x = removing[i];
        
		again(x.STU_ID);
        NID.erase(x.National_ID);
        tree.erase(x.STU_ID);
    }

    cout << "SUCCESS, DELETED RECORDS:\n";

    for (size_t i = 0; i < removing.size(); ++i){
        for (size_t j = i + 1; j < removing.size(); ++j){
            
			if (removing[j].STU_ID < removing[i].STU_ID){
                student t = removing[i]; removing[i] = removing[j]; removing[j] = t;
            }
        }
    }

    for (size_t i = 0; i < removing.size(); ++i){
        const student &x = removing[i];
        cout << "NAME:" << x.Name << ", FAMILY:" << x.Surname << ", ID:" << x.National_ID << ", GPA:" << repeat(x.GPA) << "\n";
    }
}

    void different (int STID, const string &row, const string &col){
        if (tree.empty()){ cout << "FAILED => NOT FOUND\n"; return; }
        
		map<int, student>::iterator him = tree.find(STID);
        if (him == tree.end()){ cout << "FAILED => NOT FOUND\n"; return; }
        
		if(row == "NAME") {
            
			him -> second.Name = col; cout << "SUCCESS\n"; return;
        } else if (row == "FAMILY"){
            
			him -> second.Surname = col; cout << "SUCCESS\n"; return;
        } else if (row == "GPA"){
            
			try{
                double d = atof(col.c_str()); him -> second.GPA = d; cout << "SUCCESS\n"; return;
            } catch (...){ cout << "FAILED => NOT FOUND\n"; return; }
        
		} else if (row == "ID"){
            int New_STID;
            
			try{ New_STID = atoi(col.c_str()); } catch(...){ cout << "FAILED => NOT FOUND\n"; return; }
            if (New_STID < MIN_SID){ cout << "FAILED => NOT FOUND\n"; return; }
            
			if (New_STID == STID) { cout << "SUCCESS\n"; return; }
            if (tree.count(New_STID)) { cout << "FAILED => RECORD EXISTS\n"; return; }
            
			student x = him -> second;
            tree.erase(him);
            again(STID);
            x.STU_ID = New_STID;
            
			tree[New_STID] = x;
            cout << "SUCCESS\n"; return;
        
		} else {
            cout << "FAILED => NOT FOUND\n"; return;
        }
    }

    void show (const student &x){
        
		cout << "ID: " << x.National_ID << ",\n";
        cout << "NAME: " << x.Name << ",\n";
        cout << "FAMILY: " << x.Surname << ",\n";
        cout << "STU_ID: " << x.STU_ID << ",\n";
        cout << "GPA: " << repeat(x.GPA) << "\n";
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    const string File = "Students_DB.txt";
    DBase DB; DB.loading(File);

    while (true){
        
		string UKnown;
        if (!getline(cin, UKnown)) break;

        string x = trim(UKnown);
        if (x.empty()) continue;

        if (x == "FIND BESTGPA"){
            DB.rank_GPA();
            
			continue;
        }

        if (swith(x, "FIND_GE ")){
            string z = trim(x.substr(strlen("FIND_GE ")));
            
			try{
                double y = atof(z.c_str());
                DB.Avg_STU(y);
            
			} catch(...) { cout << "FAILED => NOT FOUND\n"; }
            
			continue;
        }

        if (swith(x, "FIND ")){
            string z = trim(x.substr(5));
            
			try{
                int STID = atoi(z.c_str());
                DB.srch(STID);
            
			} catch(...) { cout << "NOT FOUND\n"; }
            
			continue;
        }

        if (swith(x, "REGISTER ")){
            string crap = trim(x.substr(strlen("REGISTER ")));
            vector<string> terrain = distinguish(crap, ',');

            string NAME, SURNAME, NatID; double GPA = 0.0; bool ok = true;
            for (size_t i = 0; i < terrain.size(); ++i){
                
				string T = trim(terrain[i]);
                size_t position = T.find(':');
                
				if (position == string::npos){ ok = false; break; }
                string key = trim(T.substr(0, position));
                string val = trim(T.substr(position + 1));
                
				if (!val.empty() && val.back() == ',') val.erase(val.end()-1);
                if (key == "NAME") NAME = val;
                
				else if (key == "FAMILY") SURNAME = val;
				else if (key == "ID") NatID = val;
                
				else if (key == "GPA"){
                    GPA = atof(val.c_str());
                
				} else { ok = false; break; }
            }

            if (!ok || NAME.empty() || SURNAME.empty() || NatID.empty()){
                cout << "FAILED => NOT FOUND\n";
            
			} else {
                DB.submit_STU(NAME, SURNAME, NatID, GPA);
            }
            
			continue;
        }

        if (x == "DELETE SUSPENDED"){
            
			DB.below_GPA();
            continue;
        }

        if (swith(x, "DELETE ")){
            
			string z = trim(x.substr(7));
            try{
                
				int STID = atoi(z.c_str());
                DB.remove(STID);
            } catch(...) { cout << "FAILED => NOT FOUND\n"; }
            
			continue;
        }

        if (swith(x, "CHANGE ")){
            
			string other = trim(x.substr(7));
            size_t one = other.find(' ');
            
			if (one == string::npos) { cout << "FAILED => NOT FOUND\n"; continue; }
            size_t two = other.find(' ', one + 1);
            
			if (two == string::npos) { cout << "FAILED => NOT FOUND\n"; continue; }
            string sides = trim(other.substr(0, one));
            string row   = trim(other.substr(one + 1, two - one - 1));
            
			string col   = trim(other.substr(two + 1));
            try {
                
				int STID = atoi(sides.c_str());
                DB.different(STID, row, col);
            } catch(...) { cout << "FAILED => NOT FOUND\n"; }
            
			continue;
        }

        cout << "FAILED => NOT FOUND\n";
    }

    DB.save(File);
    return 0;
}
