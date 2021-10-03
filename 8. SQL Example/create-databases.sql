CREATE DATABASE walmart_example;        -- Creates data base
USE walmart_example; 					-- Assigns data base to be used

CREATE TABLE locations (				-- Creates a table
	name_ID VARCHAR(255) NOT NULL,   	-- Add a string column to the table
    PRIMARY KEY (name_ID)				-- Adds a primary key as defined by the name
);

CREATE TABLE distances (
	id INT NOT NULL AUTO_INCREMENT,		-- Adds an ID column, not null, and with auto increment (will be used as primary key)
    distance DOUBLE NOT NULL,			-- Adds double column
    id_source VARCHAR(255) NOT NULL,
    id_destination VARCHAR(255) NOT NULL,
    PRIMARY KEY (id),	
    FOREIGN KEY(id_source) REFERENCES locations (name_ID)	-- Adds foreign key (name_ID) from a different table (locations)
);


INSERT INTO locations (name_ID)              -- Insert values (rows) to a table
VALUES 	('Las Vegas'), ('San Jose'), ('Phoenix'), ('Seattle'), ('El Paso');
    
INSERT INTO distances (id_source, id_destination, distance)
VALUES 	('Las Vegas', 'Las Vegas', 0),
		('Las Vegas', 'San Jose', 142), 
		('Las Vegas', 'Phoenix', 435), 
		('Las Vegas', 'Seattle', 87), 
		('Las Vegas', 'El Paso', 205),  
		('San Jose', 'Las Vegas', 142), 
        ('San Jose', 'San Jose', 0), 
        ('San Jose', 'Phoenix', 147), 
        ('San Jose', 'Seattle', 208), 
        ('San Jose', 'El Paso', 328), 
        ('Phoenix', 'Las Vegas', 435), 
        ('Phoenix', 'San Jose', 147), 
        ('Phoenix', 'Phoenix', 0), 
        ('Phoenix', 'Seattle', 284), 
        ('Phoenix', 'El Paso', 67), 
        ('Seattle', 'Las Vegas', 87), 
        ('Seattle', 'San Jose', 208), 
        ('Seattle', 'Phoenix', 284), 
        ('Seattle', 'Seattle', 0), 
        ('Seattle', 'El Paso', 139), 
        ('El Paso', 'Las Vegas', 205),
        ('El Paso', 'San Jose', 328),
        ('El Paso', 'Phoenix', 67),
        ('El Paso', 'Seattle', 139),
        ('El Paso', 'El Paso', 0);
            
