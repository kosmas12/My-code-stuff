package toastCalculator;      /* Code libraries*/

import java.util.Arrays;
import java.util.Scanner;

public class Cooker { //Start the program 
	
	static void Print(String str) //Shortcut for System.out.println()
	{
		 System.out.println(str);	
	}
	
	static void Sorry() //Tells the user they can't make toasts
	{
		Print("You can't make toasts");
	}
	
	static void Exit(int stat) //Shortcut for System.exit()
	{
		System.exit(stat);
	}
	
	public static void main(String[] args) { //main() method. This is where our main code lies
		Print("Welcome to Toast Calculator! Starting up..."); //Talk to the user
		Scanner userInput = new Scanner(System.in); //We create a Scanner (input scanner)
		int cheeseAmount; /* Declare some variables */
		int hamAmount;
		int butterAmount;
		int breadAmount;
		int timesCheese;
		int timesHam;
		int timesButter;
		int timesBread;
		int minCheese = 1;
		int minHam= 1;
		int minButter = 100; //mg (milligrams)
		int minBread = 2;
		int[] times = new int [4]; //Create an integer array that takes 4 elements
	    
		
		/* Ask for cheese */
		Print("How many slices of cheese do you have? ");
		
		cheeseAmount = userInput.nextInt(); //Tell the scanner to store the next integer the user enters
		if (cheeseAmount == 0) //If there isn't cheese
		{
			Sorry(); //Tell the user they can't make toasts
			Exit(0); //Exit with error code 0 (No issues found)
		}
		Print(cheeseAmount + " slices of cheese"); //Tell the user how many slices of cheese he/she has
		//Same things happen in the entire program
		
		/* Ask for ham */
		Print("How many slices of ham do you have? ");
		
		hamAmount = userInput.nextInt();
		
		if (hamAmount == 0)
		{
			Sorry();
			Exit(0);
		}
		
		Print(baconAmount + " slices of ham");
		
		/* Ask for butter */
		Print("How many mg (milligrams) of butter do you have? ");
		
		butterAmount = userInput.nextInt();
		
		if (butterAmount == 0) 
		{
			Sorry(); 
			Exit(0);
		}
		
		Print(butterAmount + " mg of butter");
		
		/* Ask for bread */
		Print("How many slices of bread do you have?");
		
		breadAmount = userInput.nextInt();
		
		if (breadAmount == 0) 
		{
			Sorry(); 
			Exit(0);
		}
		
		Print(breadAmount + " slices of bread");
		
		userInput.close(); //Close the scanner
		
		/* CALCULATION TIME!!! */
			
			timesCheese = cheeseAmount / minCheese; //timesCheese is our amount of cheese / minimum cheese
			timesHam = hamAmount / minHam; //same for ham
			timesButter = butterAmount / minButter; //same for butter
			timesBread = breadAmount / minBread; //same for bread
			
			times[0] = timesCheese; //We set the first element (element number 0) of times[] to timesCheese
			times[1] = timesHam; //set the 2nd element to timesHam
			times[2] = timesButter; //3rd element to timesButter
			times[3] = timesBread; //and 4th element to timesBread
		    Arrays.sort(times); //We sort the list from smallest number to highest
		    int smallest = times[0]; //From sorting, the first element is now the ingredient we have the least amount of
	        
		    // All this just outputs a really long message
			Print("You have " + timesCheese + " times cheese, " + timesHam + " times Ham, " + timesButter + " times butter, and " + timesBread + " times the bread required for a toast.");
			Print("You can make " + smallest + " toasts"); //Finally say how many toasts we can make (the times we have the least ingredient)
		}

	}
