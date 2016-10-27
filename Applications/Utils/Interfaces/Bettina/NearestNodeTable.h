/*
 * NearestNodeTable.h
 *
 *  Created on: Oct 25, 2016
 *      Author: waltherm
 */

#ifndef APPLICATIONS_UTILS_INTERFACES_BETTINA_NEARESTNODETABLE_H_
#define APPLICATIONS_UTILS_INTERFACES_BETTINA_NEARESTNODETABLE_H_

#include <vector>

class Tree;

enum class NearestNodeTableClass {
	Crown, Root, Vicinity
};

class NearestNodeTable {
public:
	NearestNodeTable(Tree *aTree, NearestNodeTableClass nntc);
	virtual ~NearestNodeTable();

	//const std::vector<std::vector<std::size_t>>& getNearestNodeTable() const;
	const std::vector<std::size_t> getNearestNodeList();

	Tree* getTheTree() const {
		return _theTree;
	}

	//void addNodeList(std::vector<std::size_t> nearestNodeList);

private:
	std::vector<std::vector<std::size_t>> _nearestNodeTable; //for faster node searching
	Tree *_theTree;
	NearestNodeTableClass _nntc;
	const double _iniRadius;
	const double _minEdgeLength;

	void initializeNearestNodeTable();
	void appendNearestNodeTable();
	double getIniRadius() const;
	double getCurrentRadius() const;
	double getMinEdgeLength() const;

};

#endif /* APPLICATIONS_UTILS_INTERFACES_BETTINA_NEARESTNODETABLE_H_ */
