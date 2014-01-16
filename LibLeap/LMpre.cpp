/*
 * LMpre.cpp
 *
 *  Created on: 04-01-2014
 *      Author: oli
 */

#include "LMpre.h"

namespace LMpre {

using namespace std;


bool LMpre::checkNear(int frameId, int fingerId) {


	int count = 0;
	GestureFrame fr;

	for (int i = max(frameId - radius, radius); i <=	 min(frameId + radius, (int) frames.size()-radius); i++) {

		fr = frames[i];
		for (int j = 0; j < fr.getHandCount(); ++j) {
			for (int k = 0; k < fr.getHand(j)->getFingerCount(); k++) {
				if (fr.getHand(j)->getFinger(k)->getId() == fingerId)
					count++;
			}
		}
	}

	if (count > radius) {
		return true;
	} else {
		return false;
	}
}

bool LMpre::isThereFinger(int fingerId, int frame) {

	GestureFrame fr = frames[frame];

	for(int i=0; i<fr.getHandCount(); i++) {

		for (int j = 0; j<fr.getHand(i)->getFingerCount(); ++j) {
			if(fr.getHand(i)->getFinger(j)->getId() == fingerId)
				return true;
		}

	}

	return false;

}

Vertex LMpre::weightedAverage(Vertex v1, Vertex v2, int w1, int w2) {

	Vertex out;

	out.setX((w1*v1.getX() + w2*v2.getX()) / (double)(w1+w2));
	out.setY((w1*v1.getY() + w2*v2.getY()) / (double)(w1+w2));
	out.setZ((w1*v1.getZ() + w2*v2.getZ()) / (double)(w1+w2));

	return out;

}


GestureFinger LMpre::averageFingers(GestureFinger *f1, GestureFinger *f2, int d1, int d2) {

	GestureFinger f;

	f.setTipPosition(weightedAverage(f1->getTipPosition(), f2->getTipPosition(), d1, d2));
	f.setStabilizedTipPosition(weightedAverage(f1->getStabilizedTipPosition(), f2->getStabilizedTipPosition(), d1, d2));
	f.setDirection(weightedAverage(f1->getDirection(), f2->getDirection(), d1, d2));
	f.setId(f1->getId());
	f.setLength((d1*f1->getLength()+d2*f2->getLength())/d1+d2);
	f.setWidth((d1*f1->getWidth()+d2*f2->getWidth())/d1+d2);

	return f;

}

GestureFinger LMpre::interpolateFinger(int fingerId, int frame) {

	int fBefore = frame-1;
	int fAfter = frame+1;

	GestureFinger f;
	GestureFinger *prec, *succ;


	while(!isThereFinger(fingerId, fBefore)) {fBefore--;}
	while(!isThereFinger(fingerId, fAfter)) {fAfter++;}

	for(int i=0;i<frames[fBefore].getHandCount(); i++) {
		for (int j = 0; j < frames[fBefore].getHand(i)->getFingerCount(); ++j) {
			if(frames[fBefore].getHand(i)->getFinger(j)->getId() == fingerId) {
				prec = frames[fBefore].getHand(i)->getFinger(j);
			}
		}
	}

	for(int i=0;i<frames[fAfter].getHandCount(); i++) {
		for (int j = 0; j < frames[fAfter].getHand(i)->getFingerCount(); ++j) {
			if(frames[fAfter].getHand(i)->getFinger(j)->getId() == fingerId) {
				succ = frames[fAfter].getHand(i)->getFinger(j);
			}
		}
	}

	int fr = fAfter-frame, bw = frame-fBefore;

	f = averageFingers(prec, succ, bw, fr);

	return f;
}

GestureFinger LMpre::cloneFinger(GestureFinger *f) {

	GestureFinger of(f->getId(), f->getStabilizedTipPosition(), f->getStabilizedTipPosition(), f->getDirection(), f->getLength(), f->getWidth());
	return of;

}

GestureFinger *LMpre::getFinger(int fingerId, int frame) {

	for(int i=0;i<frames[frame].getHandCount(); i++) {
		for (int j = 0; j < frames[frame].getHand(i)->getFingerCount(); ++j) {
			if(frames[frame].getHand(i)->getFinger(j)->getId() == fingerId) {
				return frames[frame].getHand(i)->getFinger(j);
			}
		}
	}
}

int LMpre::getHandIdFromFinger(int fingerId, int frame, bool deep=true) {

	for(int i=0;i<frames[frame].getHandCount(); i++) {
		for (int j = 0; j < frames[frame].getHand(i)->getFingerCount(); ++j) {
			if(frames[frame].getHand(i)->getFinger(j)->getId() == fingerId) {
				return frames[frame].getHand(i)->getId();
			}
		}
	}

	if(deep == false) return -1;

	int r = radius-1;

	while(r>0 && frame >0) {
		frame--;
		r--;
		for(int i=0;i<frames[frame].getHandCount(); i++) {
			for (int j = 0; j < frames[frame].getHand(i)->getFingerCount(); ++j) {
				if(frames[frame].getHand(i)->getFinger(j)->getId() == fingerId) {
					return frames[frame].getHand(i)->getId();
				}
			}
		}
	}
	return -1;
}

GestureHand LMpre::copyHand(GestureHand *source) {

	GestureHand h(source->getId(), source->getPalmPosition(), source->getStabilizedPalmPosition(), source->getPalmNormal(), source->getDirection());

	return h;

}


LMpre::LMpre(vector<GestureFrame> frames, int radius) {

	this->frames = frames;
	this->radius = radius;

}

void LMpre::fillFidVectors() {

	GestureFrame fr;
	GestureHand *ha;
	GestureFinger *fi;

	for (unsigned int i = 0; i < frames.size(); i++) {

		fr = frames[i];
//		cout << "Frame: " << i << " ";

		for (int j = 0; j < fr.getHandCount(); j++) {


			ha = fr.getHand(j);
//			cout << "Hand[" << j << "] id: " << ha->getId() << " ";
			vector<int> s;

			for (int k = 0; k < ha->getFingerCount(); k++) {

				fi = ha->getFinger(k);
//				cout << "Finger[" << k << "] id: " << fi->getId() << " ";
				s.push_back(fi->getId());

			}

			fids.push_back(s);

		}
//		cout << endl;

	}
}

vector<GestureFrame> LMpre::process() {

	fillFidVectors();

	for (unsigned int i = 0; i < fids.size(); ++i) {
		for (unsigned int j = 0; j < fids[i].size(); ++j) {
			uniqueFingers.insert(fids[i][j]);
		}
	}

	bool shouldBe, isThere;

	for (int i = 0; i < frames.size(); ++i) {
		GestureFrame g;
		for (int j = 0; j < frames[i].getHandCount(); ++j) {
			g.setTimestamp(frames[i].getTimestamp());
			g.addHand(copyHand(frames[i].getHand(j)));

		}
		outFrames.push_back(g);
	}

	GestureFrame f;
	int handId;

	// for all unique fingers
	for (set<int>::iterator it = uniqueFingers.begin(); it != uniqueFingers.end(); ++it) {

		// for a finger, go through all frames
		for (unsigned int i = 0; i < frames.size(); i++) {

			// skipping ones on the start and end
			if(i<radius || i>(frames.size()-radius)) {

				// just attach the finger to the hand...
				handId = getHandIdFromFinger(*it, i, false);
				for (int var = 0; var < outFrames[i].getHandCount(); ++var) {
					if(outFrames[i].getHand(var)->getId() == handId) {
						outFrames[i].getHand(var)->addFinger(cloneFinger(getFinger(*it, i)));
					}
				}
			}
			else {

				// perform median filter check

				shouldBe = checkNear(i, *it);
				isThere = isThereFinger(*it, i);

				if(shouldBe == isThere && shouldBe == true) {
					handId = getHandIdFromFinger(*it, i);
					if(handId != -1) {
						for (int var = 0; var < outFrames[i].getHandCount(); ++var) {
							if(outFrames[i].getHand(var)->getId() == handId) {
								outFrames[i].getHand(var)->addFinger(cloneFinger(getFinger(*it, i)));
							}
						}
					}
				}

				if(shouldBe!=isThere)  {
					if(shouldBe) {
						handId = getHandIdFromFinger(*it, i);
						for (int var = 0; var < outFrames[i].getHandCount(); ++var) {
							if(outFrames[i].getHand(var)->getId() == handId) {
								outFrames[i].getHand(var)->addFinger(interpolateFinger(*it, i));
							}
						}

					}
					else {
						// don't add
					}
				}
			}
		}
	}

	return outFrames;

}

void LMpre::displayRes() {

	GestureFrame fr;
	GestureHand *ha;
	GestureFinger *fi;

	for (unsigned int i=0; i < frames.size(); i++) {

		fr = frames[i];

		for (int j = 0; j < fr.getHandCount(); j++) {

			ha = fr.getHand(j);
			cout << ha->getId() << ": ";

			for (int k = 0; k < ha->getFingerCount(); k++) {

				fi = ha->getFinger(k);
				cout << fi->getId() << ", ";

			}

			cout << endl;

		}

	}

	cout << "---------------------------------------" << endl;
	cout << outFrames.size() << endl;


	for (unsigned int i=0; i < outFrames.size(); i++) {

		fr = outFrames[i];

		for (int j = 0; j < fr.getHandCount(); j++) {

			ha = fr.getHand(j);
			cout << ha->getId() << ": ";

			for (int k = 0; k < ha->getFingerCount(); k++) {

				fi = ha->getFinger(k);
				cout << fi->getId() << ", ";

			}

			cout << endl;

		}

	}

}


} /* namespace LMpre */
