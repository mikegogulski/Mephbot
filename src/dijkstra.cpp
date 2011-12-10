//////////////////////////////////////////////////////////////////////////////
//
// COPYRIGHT
//
// This program is Copyright (c) 2002 by Mike Gogulski <mike@gogulski.com>
//
//////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////
//
// Syadasti's open-source human-readable software LICENSE
//
// This code is freely redistributable, provided that all credits and
// copyright notices are preserved.
//
// You may use this code in your own software without restriction, provided
// that your software is distributed with complete source code.
//
// You agree by making use of this code that whatever problems you experience
// as a result are your fault, and yours alone.
//
//////////////////////////////////////////////////////////////////////////////
#define WIN32_LEAN_AND_MEAN
#include "util.h"
#include "logging.h"
#include "dijkstra.h"

class Vertex {
public:
	int Adjacent[MAX];				// adjacent vertices
	int NumAdjacencies;				// # of adjacent vertices
	double AdjacencyWeight[MAX];	// weights of adjacent vertices
	
	// these two will be undefined until dijkstra() is called
	int Previous;					// previous hop on path from source
	double Cost;					// Cost to here from source
	
	Vertex() {
		NumAdjacencies = 0; }

	void push(int neighbor, double Cost) {
		Adjacent[NumAdjacencies] = neighbor;
		AdjacencyWeight[NumAdjacencies] = Cost;
		NumAdjacencies++; } };

void dijkstra(Vertex *v, int size, int source) {
	double Cost[MAX];
	int done[MAX];
	int to_do = size;
	int i;
	for (i = 0; i < size; i++) {
		v[i].Cost = Cost[i] = INFINITY; v[i].Previous = -1; done[i] = 0; }
	Cost[source] = 0;
	while (to_do) {
		int iMin;
		for (i = 0; i < size; i++) {
			if (!done[i]) {
				iMin=i;
				break; } }
		for (i = iMin + 1; i < size; i++)
			if (!done[i] && Cost[i] < Cost[iMin]) {
				iMin = i; }
			done[iMin] = 1;
			to_do--;
			for (i = 0; i < v[iMin].NumAdjacencies; i++)
				if (Cost[iMin] + v[iMin].AdjacencyWeight[i] < Cost[v[iMin].Adjacent[i]]) {
					v[v[iMin].Adjacent[i]].Previous = iMin;
					v[v[iMin].Adjacent[i]].Cost = Cost[v[iMin].Adjacent[i]] =
						Cost[iMin] + v[iMin].AdjacencyWeight[i]; } } }

myPOINT Points[MAX];
BYTE bPointsIndex = 2;		// start at 2 to avoid PLAYER and STAIRS

WORD StairsPath[MAX][2];
BYTE bStairsPathIndex = 0;
int HopsToStairs = 0;

VOID CreatePath(Vertex *v, int source, int dest) {
	if (source == dest) {
		StairsPath[bStairsPathIndex][0] = Points[dest].x;
		StairsPath[bStairsPathIndex][1] = Points[dest].y;
		bStairsPathIndex++; }
	else {
		if (v[dest].Previous != -1) {
			CreatePath(v, source, v[dest].Previous);
			StairsPath[bStairsPathIndex][0] = Points[dest].x;
			StairsPath[bStairsPathIndex][1] = Points[dest].y;
			bStairsPathIndex++; }
		else {
			dprintf(DEBUG_ALL, "ABORT: Unable to build path to stairs");
			NumAborts++;
			Tick1 = 0;
			ChangeState(STATE_EXIT_GAME);
			ExitGame(); } }
	dprintf(DEBUG_DIJKSTRA, "bStairsPathIndex = %d", bStairsPathIndex); }

VOID FindStairsPath(VOID) {
	Vertex v[MAX];
	DWORD d;
	
	//Points[PLAYER].x = PLAYERX;
	//Points[PLAYER].y = PLAYERY;
	Points[PLAYER].x = WpDurance2X;
	Points[PLAYER].y = WpDurance2Y;
	Points[STAIRS].x = StairsX;
	Points[STAIRS].y = StairsY;

	// Create the graph
	dprintf(DEBUG_DIJKSTRA, "bPointsIndex = %d", bPointsIndex);
	
	for (int i = 0; i < bPointsIndex; i++) {
		for (int j = 0; j < bPointsIndex; j++) {
			if (i != j) {
				d = Dist2(Points[i].x, Points[i].y, Points[j].x, Points[j].y);
				//dprintf(DEBUG_DIJKSTRA, "d = %d", (int)sqrt((double)d));
				if (d <= (DWORD)(2 * 60 * 60) /*&& (Points[i].x == Points[j].x || Points[i].y == Points[j].y)*/) {
					//dprintf(DEBUG_DIJKSTRA, "adding arc to %d (%.4X, %.4X) to Vertex %d (%.4X, %.4X)", j, Points[j].x, Points[j].y, i, Points[i].x, Points[i].y);
					v[i].push(j, sqrt((double)d)); } } }
		dprintf(DEBUG_DIJKSTRA, "Vertex %d at (%.4X, %.4X) has %d arcs", i, Points[i].x, Points[i].y, v[i].NumAdjacencies); }
	
	dprintf(DEBUG_DIJKSTRA, "Calling dijkstra()");
	dijkstra(v, bPointsIndex, PLAYER);
	bStairsPathIndex = 0;
	dprintf(DEBUG_DIJKSTRA, "Calling CreatePath()");
	CreatePath(v, PLAYER, STAIRS);
	HopsToStairs = bStairsPathIndex;
	dprintf(DEBUG_DIJKSTRA, "Player at (%.4X, %.4X)", PLAYERX, PLAYERY);
	for (int k = 0; k < bStairsPathIndex; k++) {
		dprintf(DEBUG_DIJKSTRA, "Hop %d: (%.4X, %.4X)", k, StairsPath[k][0], StairsPath[k][1]); }
	dprintf(DEBUG_DIJKSTRA, "Stairs at (%.4X, %.4X)", StairsX, StairsY); }
