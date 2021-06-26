#include "DungeonGenerator.h"
//#include <Editor/UnrealEd/Classes/Editor/EditorEngine.h>

// Sets default values
ADungeonGenerator::ADungeonGenerator()
{

}

// Called when the game starts or when spawned
void ADungeonGenerator::BeginPlay()
{
	Super::BeginPlay();

	GenerateRooms();

	CalculateCenters();

	ConstructGraph();

	GenerateHalls();

	BuildDungeon();

	GetWorld()->SpawnActor<AActor>(PortalPointClass, FVector(Nodes[0].Center.X * TileOffset, Nodes[0].Center.Y * TileOffset, 100.f), FRotator(0.f));

	GetWorld()->SpawnActor<AActor>(EndGamePortal, FVector(Nodes[1].Center.X * TileOffset, Nodes[1].Center.Y * TileOffset, 100.f), FRotator(0.f));
}

// Called every frame
void ADungeonGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


// Build rooms with random dimensions and locations and add them to a collection of rooms
void ADungeonGenerator::GenerateRooms() {

	Room temp;
	int roomCount = 0;
	for (int i = 0; i < NumberOfRooms; i++) {
		for (int j = 0; j < PlacementAttempts; j++) {

			temp.Position.X = FMath::RandRange(0, MapSize);
			temp.Position.Y = FMath::RandRange(0, MapSize);
			temp.Size.X = FMath::RandRange(MinRoomSize, MaxRoomSize);
			temp.Size.Y = FMath::RandRange(MinRoomSize, MaxRoomSize);

			if (!Overlap(temp)) {
				temp.index = roomCount;
				roomCount++;
				Rooms.push_back(temp);
				break;
			}
		}
	}
}

// Calculate the center of each room
void ADungeonGenerator::CalculateCenters() {
	for (int i = 0; i < Rooms.size(); i++) {
		Rooms[i].Center.X = Rooms[i].Position.X + (Rooms[i].Size.X / 2);
		Rooms[i].Center.Y = Rooms[i].Position.Y + (Rooms[i].Size.Y / 2);
	}
}

// Convert collection of rooms into connected graph 
void ADungeonGenerator::ConstructGraph() {

	// 1. move node from Rooms to visited, entry point for the graph
	for (int i = 0; i < Rooms.size(); i++) {
		if (Rooms[i].index == 0) {
			Nodes.push_back(Rooms[i]);
			Rooms.erase(Rooms.begin() + i);
		}
	}

	// 2. iterate over all visited nodes, and find the unvisited node closest to it
	Edge temp;
	while (Rooms.size() > 0) {
		temp = CalculateShortesEdgeDistance(Nodes, Rooms);

		// add that edge to list of edges
		Edges.push_back(temp);

		for (int i = 0; i < Rooms.size(); i++) { // find bIndex in unvisited
			if (Rooms[i].index == temp.bIndex) {
				Nodes.push_back(Rooms[i]);
				Rooms.erase(Rooms.begin() + i);
				break;
			}
		}

	}

	Rooms = Nodes; // repopulate Rooms (it is used later to place the actual tiles in world, Nodes is just a graph representation)
}


// Spawn all game objects in the world (rooms, hallways, enemies)
void ADungeonGenerator::BuildDungeon() {
	for (int i = 0; i < Rooms.size(); i++) {
		for (int x = 0; x < Rooms[i].Size.X; x++) {
			for (int y = 0; y < Rooms[i].Size.Y; y++) {
				GetWorld()->SpawnActor<AActor>(FloorTileClass, FVector((Rooms[i].Position.X + x) * TileOffset, (Rooms[i].Position.Y + y) * TileOffset, 0.f), FRotator(0.f));

				float r = FMath::RandRange(0, 100);
				//TODO spawn enemy function()
				if (r > 100 - EnemySpawnChance) {
					GetWorld()->SpawnActor<AActor>(Enemy, FVector((Rooms[i].Position.X + x) * TileOffset, (Rooms[i].Position.Y + y) * TileOffset, 50.f), FRotator(0.f));
				}

				r = FMath::RandRange(0, 100);
				if (r > 100 - EnemySpawnChance) {
					GetWorld()->SpawnActor<AActor>(Potion, FVector((Rooms[i].Position.X + x) * TileOffset, (Rooms[i].Position.Y + y) * TileOffset, 50.f), FRotator(0.f));
				}

			}
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Halls:%d"), Halls.size());

	for (int i = 0; i < Halls.size(); i++) {
		for (int x = 0; x < Halls[i].Size.X; x++) {
			for (int y = 0; y < Halls[i].Size.Y; y++) {
				GetWorld()->SpawnActor<AActor>(FloorTileClass, FVector((Halls[i].Position.X + x) * TileOffset, (Halls[i].Position.Y + y) * TileOffset, 0.f), FRotator(0.f));
			}
		}
	}
}

// For each edge in the graph, generate a hallway
void ADungeonGenerator::GenerateHalls() {
	Room a;
	Room b;

	for (int i = 0; i < Edges.size(); i++) { 

		for (int j = 0; j < Nodes.size(); j++) { // get a handle on pairs of rooms connected by the current edge Edge[i]
			if (Nodes[j].index == Edges[i].aIndex) {
				a = Nodes[j];
			}
		}

		for (int j = 0; j < Nodes.size(); j++) { // get a handle on pairs of rooms connected by the current edge Edge[i]
			if (Nodes[j].index == Edges[i].bIndex) {
				b = Nodes[j];
			}
		}

		if (CheckHorizontalOverlap(a, b)) {
			UE_LOG(LogTemp, Warning, TEXT("Horizontal Overlap"));
		}
		else if (CheckVerticalOverlap(a, b)) {
			UE_LOG(LogTemp, Warning, TEXT("Vertical Overlap"));
		}
		else {
			CornerHallway(a, b);
			UE_LOG(LogTemp, Warning, TEXT("Corner Hallway"));
		}


	}
}

void ADungeonGenerator::CornerHallway(Room a, Room b) {
	int randY = FMath::RandRange(a.Position.Y, (a.Position.Y + a.Size.Y));
	int randX = FMath::RandRange(b.Position.X, (b.Position.X + b.Size.X));

	Room hallA;
	hallA.Position.Y = randY;
	hallA.Size.Y = 1;
	Room hallB;
	hallB.Position.X = randX;
	hallB.Size.X = 1;

	if (a.Position.X > b.Position.X) { // b is to the right of a
		hallA.Position.X = randX;
		hallA.Size.X = a.Position.X - randX;

		if (a.Position.Y < b.Position.Y) { // b is above a
			hallB.Position.Y = randY + 1;
			hallB.Size.Y = b.Position.Y - hallB.Position.Y;
		}
		else { // b is below a
			hallB.Position.Y = b.Position.Y + b.Size.Y;
			hallB.Size.Y = randY - hallB.Position.Y;
		}
	}
	else { // b is to the left of a
		hallA.Position.X = a.Position.X + a.Size.X;
		hallA.Size.X = randX - a.Size.X - a.Position.X + 1;

		if (a.Position.Y < b.Position.Y) { // b is above a
			hallB.Position.Y = randY;
			hallB.Size.Y = b.Position.Y - hallB.Position.Y;
		}
		else { // b is below a
			hallB.Position.Y = b.Position.Y + b.Size.Y;
			hallB.Size.Y = randY - hallB.Position.Y;
		}

	}

	Rooms.push_back(hallB);
	Rooms.push_back(hallA);
}


// Check if two rooms overlap on the vertical axis
bool ADungeonGenerator::CheckVerticalOverlap(Room a, Room b) {
	float max = FMath::Max(a.Position.Y, b.Position.Y);
	float min = FMath::Min(a.Position.Y + a.Size.Y, b.Position.Y + b.Size.Y);

	if (max < min) { // there is an overlap vertically 

		if (a.Position.X <= b.Position.X) { // a is the left of b
			UE_LOG(LogTemp, Warning, TEXT("a is left of b"));

			int hallPositionY = FMath::RandRange(min, max);

			Room hall;
			hall.Position.Y = hallPositionY;
			hall.Position.X = a.Position.X + a.Size.X;
			hall.Size.X = b.Position.X - a.Size.X - a.Position.X;
			hall.Size.Y = 1;

			Halls.push_back(hall);
		}
		else { // a is to the right of b
			UE_LOG(LogTemp, Warning, TEXT("a is right of b"));

			int hallPositionY = FMath::RandRange(min, max);

			Room hall;
			hall.Position.Y = hallPositionY;
			hall.Position.X = b.Position.X + b.Size.X;
			hall.Size.X = a.Position.X - b.Position.X - b.Size.X;
			hall.Size.Y = 1;

			Halls.push_back(hall);
		}
		return true;
	}

	return false;
}


// Check if two rooms overlap on the horizontal axis
bool ADungeonGenerator::CheckHorizontalOverlap(Room a, Room b) {
	float max = FMath::Max(a.Position.X, b.Position.X);
	float min = FMath::Min(a.Position.X + a.Size.X, b.Position.X + b.Size.X);

	if (max < min) { // there is an overlap horizontally

		if (a.Position.Y <= b.Position.Y) { // a is below b
			UE_LOG(LogTemp, Warning, TEXT("a is below b"));

			int hallPositionX = FMath::RandRange(min, max);

			Room hall;
			hall.Position.X = hallPositionX;
			hall.Position.Y = a.Position.Y + a.Size.Y;
			hall.Size.X = 1;
			hall.Size.Y = b.Position.Y - (a.Position.Y + a.Size.Y);

			Halls.push_back(hall);
		}
		else { // a is above b
			UE_LOG(LogTemp, Warning, TEXT("a is above b"));

			int hallPositionX = FMath::RandRange(min, max);

			Room hall;
			hall.Position.X = hallPositionX;
			hall.Position.Y = b.Position.Y + b.Size.Y;
			hall.Size.X = 1;
			hall.Size.Y = a.Position.Y - (b.Position.Y + b.Size.Y);

			Halls.push_back(hall);
		}
		return true;
	}
	return false;
}


// Check if two rooms overlap on either axis
bool ADungeonGenerator::Overlap(Room room) {
	// Great visual explanation of this if logic -> https://silentmatt.com/rectangle-intersection/
	for (int i = 0; i < Rooms.size(); i++) {
		if (room.Position.X < Rooms[i].Position.X + Rooms[i].Size.X + Padding) {
			if (room.Position.X + room.Size.X > Rooms[i].Position.X - Padding) {
				if (room.Position.Y < Rooms[i].Position.Y + Rooms[i].Size.Y + Padding) {
					if (room.Position.Y + room.Size.Y > Rooms[i].Position.Y - Padding) {
						return true;
					}
				}
			}
		}
	}

	return false;
}

// Calculate which two nodes in the graph are closest to eachother
Edge ADungeonGenerator::CalculateShortesEdgeDistance(std::vector<Room> visited, std::vector<Room> unvisited) {
	float minDistance = 99999999999999;
	float distance;
	Edge temp;

	for (int i = 0; i < visited.size(); i++) {
		for (int j = 0; j < unvisited.size(); j++) {
			distance = FVector::Dist2D(visited[i].Center, unvisited[j].Center);

			if (distance < minDistance) {
				minDistance = distance;
				temp.aIndex = visited[i].index;
				temp.bIndex = unvisited[j].index;
				temp.distance = distance;
			}
		}
	}

	return temp;
}


