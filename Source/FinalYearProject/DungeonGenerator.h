#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonGenerator.generated.h"

struct Room {
	int index;
	FVector Position;
	FVector Size;
	FVector Center;
};

struct Edge {
	int aIndex;
	int bIndex;
	float distance;
};

UCLASS()
class FINALYEARPROJECT_API ADungeonGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonGenerator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> FloorTileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> PortalPointClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> EndGamePortal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> Potion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	float EnemySpawnChance = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	int Padding = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	int TileOffset = 520;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	int NumberOfRooms = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	int PlacementAttempts = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	int MapSize = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	int MinRoomSize = 2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Level, meta = (AllowPrivateAccess = "true"))
	int MaxRoomSize = 5;

	std::vector<Room> Rooms;
	std::vector<Room> Halls;
	std::vector<Room> Nodes;
	std::vector<Edge> Edges;

	void GenerateRooms();

	void CalculateCenters();

	void ConstructGraph();

	void GenerateHalls();

	void BuildDungeon();

	bool Overlap(Room room);

	bool CheckHorizontalOverlap(Room a, Room b);

	bool CheckVerticalOverlap(Room a, Room b);

	void CornerHallway(Room a, Room b);

	Edge CalculateShortesEdgeDistance(std::vector<Room> visited, std::vector<Room> unvisited);
};
