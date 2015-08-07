#ifndef GENETICALGO_H
#define GENETICALGO_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <map>

#include "NeuralNet.h"

namespace gen {
	class GeneticAlgo {
	public:
		// A set of neural networks to act as a population.
		std::vector<net::NeuralNet> population;
		
		int populationSize, numberOfGenerations;
		float mutationRate, crossoverRate;

		/* Initialize a genetic algorithm from a population.
		 *
		 * Model traits are passed in such as population size, mutation rate, crossover rate, etc. */
		GeneticAlgo(int populationSize_, float mutationRate_, float crossoverRate_, std::vector<double>(*getPopulationFitness_)(std::vector<net::NeuralNet> neuralNet));
		
		/* Produce two new neural networks from two parent neural networks.
		 *
		 * The method uses the crossoverRate variable to determine if two neural networks are mixed 
		 * and then and mixes their neurons weights to produce two new neural networks. */ 
		void crossover(net::NeuralNet mom, net::NeuralNet dad, net::NeuralNet& offspring1, net::NeuralNet& offspring2);
		
		// Randomnly change the weights of a neural network based on the mutation rate.
		net::NeuralNet mutate(net::NeuralNet net);

		/* Return the most fit neural network in a population that undergoes a specified number of generations.
		 *
		 * modelNetwork is used to determine the number of inputs, outputs, hidden layers,
		 * and neurons per hidden layer for each network in the population. */
		net::NeuralNet getBestNeuralNetwork(int numberOfGenerations, net::NeuralNet modelNetwork);

		// Get the fitnesses of the neural network population.
		std::vector<double>(*getPopulationFitness)(std::vector<net::NeuralNet> neuralNet);
	};
}

#endif