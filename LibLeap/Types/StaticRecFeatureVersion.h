#ifndef STATICRECFEATUREVERSION_H_
#define STATICRECFEATUREVERSION_H_

// VERSION1 -> LP, dist 5 fin, 5 angles
// VERSION2-> LP, dist 5 fin, 5 angles, 5 dist to palms,
// VERSION3-> LP, dist 5 fin, 5 angles, 5 dist to palms, 5 angles to palm normal
// VERSION4-> (sorted) LP, dist 5 fin, 5 angles
// VERSION5-> (sorted) LP, dist 5 fin, 5 angles, 5 dist to palms
// VERSION6-> (sorted) LP, dist 5 fin, 5 angles, 5 dist to palms, 5 angles to palm normal
enum StaticRecFeatureVersion { VERSION1 = 1, VERSION2 = 2, VERSION3 = 3, VERSION4 = 4, VERSION5 = 5, VERSION6 = 6 };

#endif /* STATICRECFEATUREVERSION_H_ */
