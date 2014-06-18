//
//  Group.h
//  model3d
//
//  Created by 魏裕群 on 14-6-17.
//  Copyright (c) 2014年 魏裕群. All rights reserved.
//

#ifndef __model3d__Group__
#define __model3d__Group__
#include "Node.h"
#include <vector>

class Group: public Node{
protected:
	std::vector<Node*> m_gSubnodes;
public:
	virtual void draw();
};

#endif /* defined(__model3d__Group__) */
