#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "cities_NodeStructs.h"
#include "cities_MenuFuncs.h"
#include "cities_ListFuncs.h"
#include "cities_PrintFuncs.h"




int main(int argc, char *argv[]) {


	if (argc < 2) { //print error if two filenames are not provided
        printf("%s", "Syntax error: <exec> <filename.txt> [city_name] [city_name]\n");
		exit(1);
    }
    
    FILE *ifp = NULL;
    
    //open file
    ifp = fopen(argv[1], "r");
    
    //error checking for files
    if (ifp == NULL) {
        printf("ERROR: could not open [%s] for reading.\n", argv[1]);
        exit(1);
    }
    
    city *list = NULL;
    list = makeListFromFile(ifp, list); //master city list
    city *userList = NULL; //custom user list
        
    
    int quickUse = 0;
    
    if (argc == 3) { //one city provided -- show stats
        
        quickUse = 1;
        
        city *tmp = NULL;
        tmp = getCityByName(list, argv[2]);
        
        if (tmp != NULL) { //city name was found in list
            printf("%s", "\n");
            printCity(tmp);
            
        } else { //city name not found in list
            printf("\n>>> ERROR: \"%s\" was not found in the included data set.\n\n", argv[2]);
        }
        
        
    } else if (argc == 4) { //two cities provided -- calculate distance
        
        quickUse = 1;
        
        city *cityA = NULL;
        city *cityB = NULL;
        
        cityA = getCityByName(list, argv[2]);
        cityB = getCityByName(list, argv[3]);
        
        if (cityA == NULL){
            printf("\n>>> ERROR: \"%s\" was not found in the included data set.\n\n", argv[2]);
        } else if (cityB == NULL) {
            printf("\n>>> ERROR: \"%s\" was not found in the included data set.\n\n", argv[3]);
        } else {
            double distance = calcDistance(cityA, cityB);
            printf("\n>>> Distance between %s and %s: %.2lf miles\n\n", cityA->data->name, cityB->data->name, distance);
            
        }
        
    }

	
    

    int mainMenuChoice = 0;
    
    if (quickUse) { //don't show menu if extra command line args provided
        mainMenuChoice = MAIN_MENU_LENGTH;
    } else { //show  menu to user and get their choice
        mainMenuChoice = getMainMenuChoice();

    }
	
	
	while (mainMenuChoice != MAIN_MENU_LENGTH) { //continue if their choice is not to quit
		switch (mainMenuChoice){
			case 1:
				sortListByName(list, 1);
				printCityList(list);
				break;
			case 2:
               
			   //make or edit itinerary

                if (userList == NULL) {
                    sortListByName(list, 1);
                    printCityList_3col(list);
                    userList = makeUserCityList(list);
                }
                
                if (userList != NULL) {

                    printItinerary(userList);
                    
                    //initialize itinerary variables

                    int itinLength = 0;
					itinLength = getListLength(userList);
                    
					FILE *ofp = NULL;
                    
                    char outputFilename[100];
                    int timeStamp = 0;
					
					city *unusedCities = NULL;
					int unusedCitiesLength = 0;
               		city *temp = NULL;
               		city *temp2 = NULL;
					int cityToAdd = 0;
				
					int cityToDelete = -1;
                    char tmp[10]; //temp array to store city node to delete
                    char tmpStrCityToAdd[10]; //temp array to store city node to add 
                    
                    int itinChoice = getItinMenuChoice(); //show  menu to user and get their choice
                    int returnToMain = 0;
                    
                    while (itinChoice != 1){ //continue if their choice is not to quit
                        switch (itinChoice){
                            case 1:
								//return to main menu
                                exit(0);
                            case 2:
								//display alphabetically: A to Z
                                sortListByName(userList, 1);
                                printCityList(userList);
                                printf("%s", "\n");
                                break;
                            case 3:
								//display alphabetically: Z to A
                                sortListByName(userList, -1);
                                printCityList(userList);
                                printf("%s", "\n");
                                break;
                            case 4:
								//display by population: high to low
								itinLength = getListLength(userList);
                                sortListByPopulation(userList, 1);
								printListByPopulation(userList, itinLength);
                                break;
                            case 5:
                                //display by population: low to high
								itinLength = getListLength(userList);
                                sortListByPopulation(userList, -1);
								printListByPopulation(userList, itinLength);
                                break;
							case 6:
								//display by air quality: low to high (best to worst)
								itinLength = getListLength(userList);
								//city *lowest = findBestAirCity(userList);
								//printf("The city with the best air is: %s\n", lowest->data->name);
								selSortAirQuality_BestToWorst(userList);
								printListByAirQuality(userList, itinLength);
								break;
                            case 7:
								//display by air quality: high to low (worst to best)
								itinLength = getListLength(userList);
								selSortAirQuality_WorstToBest(userList);
								printListByAirQuality(userList, itinLength);
								break;
							case 8:
								//add city to itin
								
								//make list of cities not already in itinerary
							
								for (temp = list; temp != NULL; temp = temp->next) {
									if ( !inList(userList, temp->data->name )) {
										temp2 = makeCityNode(temp->data);
										unusedCities = insertFront(unusedCities, temp2);
									}
								}
								unusedCitiesLength = getListLength(unusedCities);
							
								if (unusedCitiesLength != 0) {

									sortListByName(unusedCities, 1);
									printCityList(unusedCities);

                                	printf("%s", "\nWhich city would you like to add?\n");
									
									while (cityToAdd < 1 || cityToAdd > unusedCitiesLength) {
										printf("Enter a number from 1 to %d: ", unusedCitiesLength);
										fgets(tmpStrCityToAdd, 10, stdin);
										sscanf(tmpStrCityToAdd, "%d", &cityToAdd);
									}

									city *tmpAdd = NULL;
									tmpAdd = getCityByNum(unusedCities, cityToAdd);
									userList = insertFront(userList, tmpAdd);
                                	printf("\n(+) %s, %s has been added to your itinerary.\n\n", tmpAdd->data->name, tmpAdd->data->country);
                                	printItinerary(userList);
									tmpAdd = NULL;
									cityToAdd = 0;
								
								} else {
									//if all cities have been added
									printf("\n>>> ERROR: There are no more cities to add.\n\n");
								}

								unusedCities = NULL;
								break;
							case 9:
								//delete city from itin
                                printItinerary(userList);
								itinLength = getListLength(userList);

                                printf("%s", "Which city would you like to delete?\n");
								while (cityToDelete < 1 || cityToDelete > itinLength) {
									printf("Enter a number from 1 to %d: ", itinLength);
									fgets(tmp, 10, stdin);
									sscanf(tmp, "%d", &cityToDelete);
								}
                                city *tmpDel = NULL;
                                tmpDel = getCityByNum(userList, cityToDelete);
                                userList = deleteCityNode(userList, tmpDel->data->name);
                                printf("\n(-) %s, %s has been removed from your itinerary.\n\n", tmpDel->data->name, tmpDel->data->country);
                                tmpDel = NULL;
                                itinLength = getListLength(userList); //updte itinerary length
                                if (itinLength < 1) {
                                    printf("%s", ">>> Your itnerary is empty.\n\nReturning to main menu...\n");
                                    returnToMain = 1;
                                        
                                           
                                } else {
                                    printItinerary(userList);
                                }

								cityToDelete = 0;
                                break;
							
                            case 10:
                                //open file for writing
                                timeStamp = (int)time(NULL);
                                snprintf(outputFilename, sizeof(outputFilename), "Itinerary_%d.txt", timeStamp);
                                ofp = fopen(outputFilename, "w");
                                
                                if (ofp != NULL) {
                                   	printItineraryToFile(userList, ofp); 
                                    fclose(ofp);
									printf("\n>>> SUCCESS: %s written to disk.\n\n", outputFilename);

                                } else {
                                    printf("%s", ">>> ERROR: Could not write file to disk.");
                                }
                               
                                break;
                                
                               
                        } //end itinerary choices switch
                        if (returnToMain == 1) {
                            itinChoice = 1;
                        } else {
                            itinChoice = getItinMenuChoice(); //show menu after their task is complete
                        }
                        
                    } //end while for itinerary menu
                    
                } /*else {
                    printf("\n%s\n", ">>> Your itinerary is empty.\n");
                }*/
				break;
			
            case 3:
                //calculate distance between cities
                sortListByName(list, 1);
                printCityList(list);
                
                char tmp[10]; //temp array to store city number
                int numForCityA = 0;
                int numForCityB = 0;
                city *A = NULL;
                city *B = NULL;

                
                printf("%s", "\nEnter number for city A: ");
                fgets(tmp, 10, stdin);
                sscanf(tmp, "%d", &numForCityA);
                
                printf("%s", "Enter number for city B: ");
                fgets(tmp, 10, stdin);
                sscanf(tmp, "%d", &numForCityB);
                
                A = getCityByNum(list, numForCityA);
                B = getCityByNum(list, numForCityB);
                
                double dist = calcDistance(A, B);
                printf("\n>>> Distance between %s and %s: %.2lf miles\n", A->data->name, B->data->name, dist);
                break;

		}
		mainMenuChoice = getMainMenuChoice(); //show menu after their task is complete
	}


	printf("%s", "Exiting program...\n");



	
	
	return 0;

}


