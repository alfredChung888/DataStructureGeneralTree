//
//  main.cpp
//  DSAassignmentGeneralTree
//
//  Created by Alfred Chung on 24/5/2022.
//

#include<iostream>
#include<stdlib.h>
#include<iomanip>
#include <string>
#include<time.h>
#include<vector>

using namespace std;
#include "item.h"
struct Node{
    Item item;
    vector<Node*> child;
};

char identity;
int numberOfChildren;
vector<Node*>::iterator iterate;
int position;
string phrase;
int keyItem=1;
int keyCategory=2;

void maintainItems(Node* node);



int keyGenerate(Node* node,int keyValue){
    if(keyValue==1){
        keyItem=keyItem+2;
        return keyItem;
    }
    else{
        keyCategory=keyCategory+2;
        return keyCategory;
    }
}


void addItem(Node* node){
    int keyNum=1;
    int key;
    Node* addNewItem;
    addNewItem=new Node;
    key=keyGenerate(node,keyNum);
    addNewItem->item.setKey(key);
    addNewItem->item.inputAnItem();
    node->child.push_back(addNewItem);
    
}

void addCategory(Node* node){
    int keyNum=2;
    int key;
    Node* addNewItem;
    addNewItem=new Node;
    key=keyGenerate(node,keyNum);
    addNewItem->item.setKey(key);
    addNewItem->item.inputAnItem();
    node->child.push_back(addNewItem);
}

void modifyItem(Node *node){
    Node* modify=NULL;
    modify=new Node;
    
    
    modify->item.setIsCategory(false);
    modify->item.inputAnItem();
    node->item.setTitle(modify->item.getTitle());
    node->item.setQuantity(modify->item.getQuantity());
    node->item.setPrice(modify->item.getPrice());
    node->item.setMaxDiscount(modify->item.getMaxDiscount());
    node->item.setDescription(modify->item.getDescription());
}

void modifyCategory(Node *node){
    Node* modify=NULL;
    modify=new Node;
    
    modify->item.setIsCategory(true);
    modify->item.inputAnItem();
    node->item.setTitle(modify->item.getTitle());
}


void search(Node* node,vector<Node*>::iterator iterate){
    bool found=false;
    int i=0;
    cout<<endl;
    cout<<"The following item/categories contain the phrase:"<<endl;

    if(!node->child.empty()){
        for(iterate=node->child.begin(); iterate!=node->child.end(); iterate++, i++){
            if(node->child[i]->item.getTitle()==phrase || node->child[i]->item.getDescription()==phrase){
                found=true;

                if(found==true){
                    node->child[i]->item.printAItem(0);
                    cout<<"found an item!"<<endl;
                    cout<<endl;
                }
                if(!node->child[i]->child.empty()){
                    search(node->child[i],iterate);
                    
                }

            }

        }
    }
}





void buyMenu(Node* node){
    int itemToBuy;
    int number;
    
    
    numberOfChildren=node->child.size();
    
    do{
        number=numberOfChildren;
        cout<<"======================="<<endl;;
        node->item.displayTitle();
        cout<<endl;
        int i=0;
        if(!node->child.empty()){
            for(iterate=node->child.begin(); iterate!=node->child.end(); iterate++, i++){
                cout<<i+1<<". ";
                node->child[i]->item.displayTitle();
                cout<<endl;
                
            }
        }
        cout<<number+1<<". Back to upper category"<<endl;
        cout<<"------------------------"<<endl;
        cout<<"Input a number from 1 to "<<number+1<<endl;
        cin>>itemToBuy;
        if(itemToBuy==number+1){
            return;
        }
        else{
            if(node->child[itemToBuy-1]->item.isCategory()){
                buyMenu(node->child[itemToBuy-1]);
            }
            else{
                cout<<"The following item is for sale:"<<endl;
                node->child[itemToBuy-1]->item.printAItem(0);
                node->child[itemToBuy-1]->item.buyASellingItem();
                if(node->child[itemToBuy-1]->item.getQuantity()==0){
                    node->child.erase(node->child.begin()+position);
                    numberOfChildren--;
                    cout<<"item is out of stock!"<<endl;
                }
                
            }
            
        }
        
        
    }while(itemToBuy!=number+1);
}

void itemMenu(Node* node, int index){
    node->child[index]->item.printAItem(0);
    int manageChoice;
    
    
    do{
        
        cout<<"====================="<<endl;
        cout<<"1: Buy the item (for buyers)"<<endl;
        cout<<"2: Delete the item (for the manager)"<<endl;
        cout<<"3: Modify the item (for the manager)"<<endl;
        cout<<"4: Other operations"<<endl;
        cout<<"====================="<<endl;
        cout<<"Input a number from 1 to 4"<<endl;
        
        cin>>manageChoice;
        
        if(manageChoice==1){
            if(identity=='b'){
                buyMenu(node);
            }
            else{
                cout<<"Only buyers can buy an item!"<<endl;
            }
        }
        else if(manageChoice==2){
            if(identity=='m'){
                node->item.confirmAction("buy");
                node->child.erase(node->child.begin() + index-1);
                return;
                //node is the parent passed in
            }
            else{
                cout<<"Only managers can delete items!"<<endl;
            }
        }
        else if(manageChoice==3){
            if(identity=='m'){
                if(node->child[index]->item.isCategory()){
                    modifyCategory(node->child[index]);
                    node->child[index]->item.printAItem(0);

                }
                else{
                    modifyItem(node->child[index]);
                    node->child[index]->item.printAItem(0);


                }
            }
            else{
                cout<<"Only managers can modify items!"<<endl;
            }
        }
        else if(manageChoice==4){
            maintainItems(node->child[index]);
            
        }
    }while(manageChoice!=4);
    
}

void categoryEmpty(Node* node, int index){
    int categoryChoice;
    
    do{
        numberOfChildren= node->child.size();
        node->child[index]->item.displayTitle();
        cout<<endl;
        
        cout<<"The above category is empty. Please choose"<<endl;
        cout<<" ====================="<<endl;
        cout<<"1. Delete the category"<<endl;
        cout<<"2. Modify the category"<<endl;
        cout<<"3. Other operations"<<endl;
        cout<<" ====================="<<endl;
        cout<<"Input a number from 1 to 3: "<<endl;
        cin>>categoryChoice;
        
        if(categoryChoice==3){
            maintainItems(node->child[index]);
        }
        else if(categoryChoice==2){
            if(identity=='m'){
            modifyCategory(node->child[index]);
            }
            else{
                cout<<"only managers can modify an item/category"<<endl;
            }
        }
        else if(categoryChoice==1){
            if(identity=='m'){
                node->item.confirmAction("buy");
                node->child.erase(node->child.begin()+index);
                numberOfChildren--;
                return;
            }
            else{
                cout<<"only managers can delete an category"<<endl;
            }
        }
        else{
            cout<<"input a valid character"<<endl;
        }
        
    }while(categoryChoice!=3);
}
void maintainItems(Node* node){
    int manageChoice;
    
    do{
        numberOfChildren= node->child.size();
        node->item.displayTitle();
        cout<<endl;
        int i=0;
        
        for(iterate=node->child.begin(); iterate!=node->child.end(); iterate++, i++){
            cout<<i+1<<". ";
            node->child[i]->item.displayTitle();
            cout<<endl;
            
        }
        if( node->item.isCategory()){
            cout<<numberOfChildren+1<<". Add a new category"<<endl;
            cout<<numberOfChildren+2<<". Add a new selling item"<<endl;
            cout<<numberOfChildren+3<<". Back to upper category"<<endl;
            cout<<"------------------------"<<endl;
            cout<<"Input a number from 1 to "<<numberOfChildren+3<<endl;
            
            
            cin>>manageChoice;
            
            if(manageChoice==numberOfChildren+3){
                return;
            }
            else if(manageChoice==numberOfChildren+2){
                addItem(node);
            }
            else if(manageChoice==numberOfChildren+1){
                addCategory(node);
            }
            else if(manageChoice<=numberOfChildren){
                if(node->child[manageChoice-1]->item.isCategory() && node->child[manageChoice-1]->child.empty()){
                    categoryEmpty(node,manageChoice-1);
                }
                else if(node->child[manageChoice-1]->item.isCategory() && !node->child.empty()){
                    maintainItems(node->child[manageChoice-1]);
                }
                else{
                    itemMenu(node,manageChoice-1);
                    position=manageChoice-1;
                }
            }
        }
        else{
            node->item.printAItem(0);
            cout<<numberOfChildren+1<<". Back to upper category"<<endl;
            cout<<"------------------------"<<endl;
            cout<<"Input a number from 1 to "<<numberOfChildren+1<<endl;
            cin>>manageChoice;
            return;
            
        }
        
    }while(manageChoice!=numberOfChildren+3);
}

void managerMenu(Node* node){
    char managerChoice;
    
    do{
        cout<<"---------Menu----------"<<endl;
        cout<<"M. Maintain the catalogue"<<endl;
        cout<<"S. Search in the catalogue"<<endl;
        cout<<"Q. Quit to main menu"<<endl;
        cout<<"Enter your choice: "<<endl;
        cout<<"------------------------"<<endl;
        managerChoice=' ';
        cin>>managerChoice;
        if(managerChoice=='m'){
            maintainItems(node);
            
        }
        else if(managerChoice=='s'){
            cout<<"Please input a phrase to look for: ";
            cin>>phrase;
            search(node,iterate);
        }
        else if(managerChoice=='q'){
            return;
        }
        else{
            cout<<"input a valid character"<<endl;
        }
        
    }while(managerChoice!='q');
}


void buyerMenu(Node* node){
    char choice;
    
    do{
        cout<<"----------Menu----------"<<endl;
        cout<<"B. Buy a selling item"<<endl;
        cout<<"S. Search an Item"<<endl;
        cout<<"Q. Quit to Main Menu"<<endl;
        cout<<"------------------------"<<endl;
        cout<<"Enter your choice: "<<endl;
        choice=' ';
        cin>>choice;
        if(choice=='b'){
            buyMenu(node);
        }
        else if(choice=='s'){
            cout<<"Please input a phrase to look for: ";
            cin>>phrase;
            search(node,iterate);
        }
        else if(choice=='q'){
            return;
        }
        else{
            cout<<"input a valid character"<<endl;
        }
        
        
    }while(choice!='q');
}


void sellerMenu(Node* node){
    char sellChoice;
    do{
        cout<<"----------Menu----------"<<endl;
        cout<<"P. Place a selling item"<<endl;
        cout<<"S. Search an Item"<<endl;
        cout<<"Q. Quit to Main Menu"<<endl;
        cout<<"------------------------"<<endl;
        cout<<"Enter your choice: "<<endl;
        
        cin>>sellChoice;
        
        if(sellChoice=='p'){
            maintainItems(node);
            
        }
        else if(sellChoice=='s'){
            cout<<"Please input a phrase to look for: ";
            cin>>phrase;
            search(node,iterate);
            
        }
        else if(sellChoice=='q'){
            return;
        }
        
    }while(sellChoice!='q');
}

void mainMenu(){
    char choice;
    Node* root=NULL;
    root=new Node;
    root->item.setTitle("main menu");
    root->item.setKey(2);
    

    Node* addNewItem;
    addNewItem=new Node;
    addNewItem->item.setKey(1);
    addNewItem->item.setQuantity(1);
    addNewItem->item.setDescription("nothing");
    addNewItem->item.setTitle("TEST");
    addNewItem->item.setPrice(20);
    addNewItem->item.setMaxDiscount(0.1);
    root->child.push_back(addNewItem);
    
    
    do{
        cout<<"-----Menu-----"<<endl;
        cout<<"S. Sell an Item"<<endl;
        cout<<"B. Buy an Item"<<endl;
        cout<<"M. Manage an Item"<<endl;
        cout<<"Q. Quit"<<endl;
        cout<<"--------------"<<endl;
        cout<<"Enter your choice: "<<endl;
        
        cin>>choice;
        identity=choice;
        if(choice=='s'){
            sellerMenu(root);
            
        }
        else if(choice=='b'){
            buyerMenu(root);
            
        }
        else if(choice=='m'){
            managerMenu(root);
        }
    }
    while(choice!='q');
    
}

int main() {
    mainMenu();
    return 0;
}

