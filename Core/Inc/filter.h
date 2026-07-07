/*
 * filter.h
 *
 *  Created on: Jul 1, 2026
 *      Author: Rafal
 */

#ifndef INC_FILTER_H_
#define INC_FILTER_H_

typedef struct {
    float alpha;
    float value;
} EMA_Filter_t;

void  EMA_Init(EMA_Filter_t *f, float alpha, float initial_value);
float EMA_Update(EMA_Filter_t *f, float input);
void EMA_Reset(EMA_Filter_t *f);

#endif /* INC_FILTER_H_ */
