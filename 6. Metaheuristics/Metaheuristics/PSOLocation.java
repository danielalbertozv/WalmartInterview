package Metaheuristics;

import java.util.ArrayList;

public class PSOLocation {

    public ArrayList<Double> locations;

    // Constructor
    public PSOLocation(ArrayList<Double> locations)
    {
        this.locations = new ArrayList<Double>();
        this.locations = locations;
    }

    public ArrayList<Double> getLocations() {
        return this.locations;
    }

    public void updateLocation(ArrayList<Double> update)
    {
        this.locations.addAll(update);
    }
}