#pragma once

// 自动把起切点分配到角上时，角上两个图形的夹角不能在180度左右这个范围内。
#define CORNER_NOT_LEAD					5.0

// 引入引出线发生干涉时，允许缩短的次数。
#define LEAD_SCALE_COUNT				5