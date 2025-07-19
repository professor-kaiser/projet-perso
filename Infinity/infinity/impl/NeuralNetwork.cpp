#include "../include/NeuralNetwork.hpp"
#include <iostream>
#include "../include/Layer/Output.hpp"

namespace Infinity
{
    NeuralNetwork::NeuralNetwork(const std::shared_ptr<Layer::Layer>& layer)
    : linput(Layer::begin(layer)), loutput(Layer::end(layer))
    {}

    NeuralNetwork::NeuralNetwork(const std::shared_ptr<Layer::Layer>& input, const std::shared_ptr<Layer::Layer>& output)
    : linput(input), loutput(output)
    {}

    void
    NeuralNetwork::train(NeuralNetwork::Dataset dataset, int epochs, double rate)
    {
        for (int epoch {0}; epoch < epochs; epoch++) 
        {
            std::cout << "[B] epoch: " << epoch << std::endl;
            for(int i {0}; i < dataset.size(); i++)
            {
                Sample X = dataset[i].first;
                Sample y = dataset[i].second;
                std::shared_ptr<Layer::Layer> input = this->linput.lock();
                std::shared_ptr<Layer::Output> output = std::dynamic_pointer_cast<Layer::Output>(this->loutput.lock());

                input->init(X);
                this->forward();
                
                output->updateTarget(y);
                
                this->backward();
                this->update(rate);
            }
            std::cout << "[E] epoch: " << epoch << std::endl;
        }
    }

    void
    NeuralNetwork::train(NeuralNetwork::Dataset dataset, int epochs, double rate, int batch_size, const std::string& savedata)
    {
        for (int epoch = 0; epoch < epochs; epoch++) 
        {
            std::cout << "[B] epoch: " << epoch << std::endl;

            for (int i = 0; i < dataset.size(); i += batch_size)
            {
                int end = std::min(i + batch_size, (int)dataset.size());
                std::cout << "[B] batch: " << i << " - " << end - 1 << std::endl;

                this->zeroGradients();
                for (int j = i; j < end; j++)
                {
                    Sample X = dataset[j].first;
                    Sample y = dataset[j].second;
                    
                    std::shared_ptr<Layer::Layer> input = this->linput.lock();
                    std::shared_ptr<Layer::Output> output = std::dynamic_pointer_cast<Layer::Output>(this->loutput.lock());

                    input->init(X);
                    this->forward();

                    output->updateTarget(y);

                    this->backward();
                    this->accumulateGradients();
                }

                this->update(rate, true);
                std::cout << "[E] batch: " << i << " - " << end - 1 << std::endl;
            }

            this->save(savedata);
            std::cout << "[E] epoch: " << epoch << std::endl;
        }
    }

    void 
    NeuralNetwork::forward() const
    {
        std::shared_ptr<Layer::Layer> it = this->linput.lock();
        while(it)
        {
            it->forward();
            it = it->getNext().lock();
        }
    }
    
    void 
    NeuralNetwork::backward() const
    {
        std::shared_ptr<Layer::Layer> it = this->loutput.lock();
        while(it)
        {
            it->backward();
            it = it->getPrevious().lock();
        }
    }

    void 
    NeuralNetwork::zeroGradients()
    {
        std::shared_ptr<Layer::Layer> it = this->linput.lock();
        while(it)
        {
            it->zeroGradients();
            it = it->getNext().lock();
        }
    }

    void 
    NeuralNetwork::accumulateGradients()
    {
        std::shared_ptr<Layer::Layer> it = this->loutput.lock();
        while(it)
        {
            it->accumulateGradients();
            it = it->getPrevious().lock();
        }
    }

    void 
    NeuralNetwork::update(double rate, bool useMiniBatch) const
    {
        std::shared_ptr<Layer::Layer> it = this->loutput.lock();
        while(it)
        {
            it->update(rate, useMiniBatch);
            it = it->getPrevious().lock();
        }
    }

    json 
    NeuralNetwork::serialize()
    {
        json jNN = json::array();
        std::shared_ptr<Layer::Layer> it = this->linput.lock();
        it = it->getNext().lock();

        while(it)
        {
            jNN.push_back(it->serialize());
            it = it->getNext().lock();
        }

        return json {{"NeuralNetwork", jNN}};
    }

    void
    NeuralNetwork::save(std::string filename)
    {
        std::ofstream file(std::string("data/").append(filename));
        json full = this->serialize();

        if(file)
        {
            file << full.dump(4);
            file.close();
        }
        else
        {
            std::cerr << "Error open file!" << std::endl;
        }
    }

    json 
    NeuralNetwork::loadJSON(std::string filename)
    {
        std::ifstream file(std::string("data/").append(filename));
        json data;
        
        if (!file.is_open()) 
        {
            std::cerr << "Error when open file: " << filename << std::endl;
        }
        
        try 
        {
            file >> data;
        } 
        catch (const std::exception& e) 
        {
            std::cerr << "parsing JSON error: " << e.what() << std::endl;
        }

        return data;
    }

    void 
    NeuralNetwork::load(std::string filename)
    {
        std::shared_ptr<Layer::Layer> it = this->linput.lock();
        it = it->getNext().lock();
        json data = this->loadJSON(filename);
        int index {0};

        while(it)
        {
            it->load(data["NeuralNetwork"][index]);
            it = it->getNext().lock();
            index++;
        }
    }
}