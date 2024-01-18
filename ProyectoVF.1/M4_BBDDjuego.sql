DROP DATABASE IF EXISTS M4_BBDDjuego;
CREATE DATABASE M4_BBDDjuego;

USE M4_BBDDjuego;

CREATE TABLE Jugador(
	Id INT PRIMARY KEY NOT NULL AUTO_INCREMENT,
	Nombre VARCHAR(20),
	Pass VARCHAR(20),
	Edad INT
)ENGINE = InnoDB;

CREATE TABLE Partida(
	Id INT PRIMARY KEY NOT NULL,
	Ganador VARCHAR(20),
	Perdedor VARCHAR(20),
	Fecha VARCHAR(20),
	PuntosGanador INT,
	PuntosPerdedor INT
)ENGINE = InnoDB;

CREATE TABLE Relacion(
	IdJugador INT,
	FOREIGN KEY(IdJugador) REFERENCES Jugador(Id),
	IdPartida INT,
	FOREIGN KEY(IdPartida) REFERENCES Partida(Id),
	Puntos INT,
	Color VARCHAR(20),
	Sacar5 INT
)ENGINE = InnoDB;

INSERT INTO Jugador (Id,Nombre,Pass,Edad) VALUES (1,'Arnau','Arnau',20);
INSERT INTO Jugador (Id,Nombre,Pass,Edad) VALUES (2,'Victor','Victor',19);
INSERT INTO Jugador (Id,Nombre,Pass,Edad) VALUES (3,'Martin','Martin',20);
INSERT INTO Jugador (Id,Nombre,Pass,Edad) VALUES (4,'Rohail','mimara',56);
INSERT INTO Jugador (Id,Nombre,Pass,Edad) VALUES (5,'Ryan','Ryan',7);
INSERT INTO Jugador (Id,Nombre,Pass,Edad) VALUES (6,'Paco','Sanz',9);
INSERT INTO Jugador (Id,Nombre,Pass,Edad) VALUES (7,'Antonia','mimara',35);

INSERT INTO Partida (Id, Ganador, Perdedor, Fecha, PuntosGanador, PuntosPerdedor) VALUES (1,'Rohail','Victor','2023-01-04',35,3);
INSERT INTO Partida (Id, Ganador, Perdedor, Fecha, PuntosGanador, PuntosPerdedor) VALUES (2,'Martin','Arnau','2022-12-13',10,6);


INSERT INTO Relacion (IdJugador, IdPartIda, Puntos, Color) VALUES (4,1,35,'rojo');
INSERT INTO Relacion (IdJugador, IdPartIda, Puntos, Color) VALUES (3,1,10,'azul');
INSERT INTO Relacion (IdJugador, IdPartIda, Puntos, Color) VALUES (2,1,3,'verde');
INSERT INTO Relacion (IdJugador, IdPartIda, Puntos, Color) VALUES (1,1,6,'amarillo');
INSERT INTO Relacion (IdJugador, IdPartIda, Puntos, Color) VALUES (3,2,10,'rojo');
INSERT INTO Relacion (IdJugador, IdPartIda, Puntos, Color) VALUES (7,2,1,'verde');
INSERT INTO Relacion (IdJugador, IdPartIda, Puntos, Color) VALUES (1,2,6,'amarillo');


