/*
 * tree.h
 *
 *  Created on: Aug 29, 2016
 *      Author: waltherm
 */

#ifndef TREE_H_
#define TREE_H_

class tree {
public:
	tree();
	virtual ~tree();

private:
	double _stemRadius;
	double _crownRadius;
	double _rootRadius;
	double _stemHeight;
	double _crownHeight;
	double _rootDepth;
	double v_leaf;
	double v_branch;
	double v_stem;
	double v_croot;
	double v_froot;
	double v_tree;
	double r1;
	double r2;
	double res_a;
	double res_b;
	double res_avail;
	double wa1;
	double wa2;
	double wb1;
	double wb2;
	double growth;
	double hg;
	double cg;
	double rg;
	double dg;
	double stepfrac;
	double countabove;
	double win_countabove;
	double countbelow;
	double win_countbelow;
	double above_c;
	double below_c;
	double deathflag;
	double mindist;
	double L_p;
	double psi_leaf;
	double k_f_sap;
	double R_solar;
	double max_h;
	double k_maint;
};

#endif /* TREE_H_ */
