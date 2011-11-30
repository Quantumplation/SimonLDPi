#include <vector>
//#include "Redirect.h"
//#include "NIDAQWrapper.h"
#include "Win32++\wincore.h"
#include "GUIBase.h"
#include "GUITrack.h"
#include "GUINode.h"
#include "GUIButton.h"
using namespace std;
using namespace SLDP;

#define TRANS(x, y) (x*xScale)+xShift, (y*yScale)+yShift
#define EDGE(x, y) track.addEdge(new GUIEdge(x, y, (x->getLabel() + y->getLabel())))

class CView : public CWnd
{
public:
	CView() :
	  objectToLabel(NULL), modeButton(640, 480, 125, 50, &track)
	{
		buttons.push_back(&modeButton);
/*		GUINode *L1, *L2, *L3;
		GUINode *Empty1, *Empty2;
		GUINode *One1, *One2, *Empty3, *Empty4, *One3, *One4;
		GUINode *Two1, *Two2, *Two3, *Two4;
		GUINode *Three, *Empty5, *Empty6;
		GUINode *Four;
		GUINode *Five1, *Five2, *Five3, *Five4;
		GUINode *Six;
		GUINode *Seven1, *Seven2, *Seven3, *Seven4;
		GUINode *Empty7, *Eight;
		GUINode *R1, *R2, *Empty8, *Empty9, *R3;
		int xShift = 50, yShift = 50, xScale = 10, yScale = 10;
		L1 = track.addNode(new GUINode(TRANS(0, 10), "L1"));
		L2 = track.addNode(new GUINode(TRANS(0, 20), "L2"));
		L3 = track.addNode(new GUINode(TRANS(0, 35), "L3"));
		Empty1 = track.addNode(new GUINode(TRANS(10, 35), "EMPTY1"));
		Empty2 = track.addNode(new GUINode(TRANS(20, 30), "EMPTY2"));
		One1 = track.addNode(new GUINode(TRANS(10, 10), "A"));
		One2 = track.addNode(new GUINode(TRANS(20, 10), "B"));
		Empty3 = track.addNode(new GUINode(TRANS(30, 10), "EMPTY3"));
		Empty4 = track.addNode(new GUINode(TRANS(35, 0), "EMPTY4"));
		One3 = track.addNode(new GUINode(TRANS(10, 20), "I"));
		One4 = track.addNode(new GUINode(TRANS(20, 20), "J"));
		Two1 = track.addNode(new GUINode(TRANS(30, 20), "K"));
		Two2 = track.addNode(new GUINode(TRANS(40, 20), "L"));
		Two3 = track.addNode(new GUINode(TRANS(30, 30), "Q"));
		Two4 = track.addNode(new GUINode(TRANS(40, 30), "S"));
		Three = track.addNode(new GUINode(TRANS(35, 30), "R"));
		Empty5 = track.addNode(new GUINode(TRANS(40, 35), "EMPTY5"));
		Empty6 = track.addNode(new GUINode(TRANS(60, 35), "EMPTY6"));
		Four = track.addNode(new GUINode(TRANS(45, 0), "C"));
		Five1 = track.addNode(new GUINode(TRANS(50, 10), "F"));
		Five2 = track.addNode(new GUINode(TRANS(60, 10), "G"));
		Five3 = track.addNode(new GUINode(TRANS(50, 20), "M"));
		Five4 = track.addNode(new GUINode(TRANS(60, 20), "N"));
		Six = track.addNode(new GUINode(TRANS(65, 30), "U"));
		Seven1 = track.addNode(new GUINode(TRANS(70, 20), "O"));
		Seven2 = track.addNode(new GUINode(TRANS(80, 20), "P"));
		Seven3 = track.addNode(new GUINode(TRANS(70, 30), "V"));
		Seven4 = track.addNode(new GUINode(TRANS(80, 30), "W"));
		Empty7 = track.addNode(new GUINode(TRANS(80, 10), "EMPTY7"));
		Eight = track.addNode(new GUINode(TRANS(85, 0), "D"));
		R1 = track.addNode(new GUINode(TRANS(100, 0), "R1"));
		R2 = track.addNode(new GUINode(TRANS(100, 20), "R2"));
		Empty8 = track.addNode(new GUINode(TRANS(85, 30), "EMPTY8"));
		Empty9 = track.addNode(new GUINode(TRANS(95, 35), "EMPTY9"));
		R3 = track.addNode(new GUINode(TRANS(100, 35), "R3"));
		EDGE(L1, One1);
		EDGE(One1, One2);
		EDGE(One3, One4);
		EDGE(One1, One4);
		EDGE(One3, One2);
		EDGE(One2, Empty3);
		EDGE(Empty3, Empty4);
		EDGE(Empty4, Four);
		EDGE(Four, Eight);
		EDGE(Eight, R1);
		EDGE(Four, Five1);
		EDGE(Five1, Five2);
		EDGE(Five3, Five4);
		EDGE(Five1, Five4);
		EDGE(Five3, Five2);
		EDGE(Five2, Empty7);
		EDGE(Empty7, Eight);
		EDGE(L2, One3);
		EDGE(One4, Two1);
		EDGE(Five4, Seven1);
		EDGE(Seven1, Seven2);
		EDGE(Seven3, Seven4);
		EDGE(Seven1, Seven4);
		EDGE(Seven3, Seven2);
		EDGE(Seven2, R2);
		EDGE(L3, Empty1);
		EDGE(Empty1, Empty2);
		EDGE(Empty2, Two3);
		EDGE(Two1, Two2);
		EDGE(Two3, Three);
		EDGE(Three, Two4);
		EDGE(Two1, Two4);
		EDGE(Two3, Two2);
		EDGE(Two2, Five3);
		EDGE(Two4, Six);
		EDGE(Three, Empty5);
		EDGE(Empty5, Empty6);
		EDGE(Empty6, Six);
		EDGE(Six, Seven3);
		EDGE(Seven4, Empty8);
		EDGE(Empty8, Empty9);
		EDGE(Empty9, R3);
		track.addSwitch(One1, One2, One3, One4, "One");
		track.addSwitch(Two1, Two2, Two3, Two4, "Two");
		track.addSwitch(Five1, Five2, Five3, Five4, "Five");
		track.addSwitch(Seven1, Seven2, Seven3, Seven4, "Seven");
		Empty1->SetLabelVisible(false);
		Empty2->SetLabelVisible(false);
		Empty3->SetLabelVisible(false);
		Empty4->SetLabelVisible(false);
		Empty5->SetLabelVisible(false);
		Empty6->SetLabelVisible(false);
		Empty7->SetLabelVisible(false);
		Empty8->SetLabelVisible(false);
		Empty9->SetLabelVisible(false);
		track.saveToFile("Blahdeblah.xml");*/
		track.loadFromFile("Blahdeblah.xml");
	}
	virtual ~CView() {}
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* dc)
	{
		for (size_t i = 0; i < buttons.size(); ++i)
			buttons[i]->draw(*dc);
		track.draw(*dc);
		if ((GetKeyState(VK_LBUTTON) & 0x8000) && track.getMode())
		{
			dc->MoveTo(lastMouseClick);
			dc->LineTo(lastMousePos);
		}
	}


private:
	GUITrack track;
	POINT lastMouseClick;
	POINT lastMousePos;
	GUIBase* objectToLabel;
	ModeButton modeButton;
	vector<GUIButton*> buttons;
};

class MyApp : public CWinApp
{
public:
	MyApp() {}
	virtual ~MyApp() {}
	virtual BOOL InitInstance()
	{
		myView.Create();
		myView.SetFont(new CFont(CreateFont(16,
			8,
			0,
			0,
			FW_DONTCARE,
			FALSE,
			FALSE,
			0,
			ANSI_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			FIXED_PITCH | FF_MODERN,
			L"Courier New")));
		return TRUE;
	}
private:
	CView myView;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR nCmdLine, int nCmdShow)
{
//	NIDAQWrapper wrapper;
//	wrapper.Initialize();
	MyApp myApp;
	return myApp.Run();
}

LRESULT CView::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		{
			if (objectToLabel)
			{
				if (wParam >= '0' && wParam <= 'Z' && wParam != 0x40 || wParam == VK_SPACE)
					objectToLabel->addCharToLabel((wchar_t)wParam);
				else if (wParam == VK_BACK)
					objectToLabel->removeCharFromLabel();
				else if (wParam != VK_SHIFT)
				{
					objectToLabel->setBeingRelabeled(false);
					objectToLabel = NULL;
				}
				InvalidateRect(NULL, true);
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetCapture();
			lastMousePos.x = lastMouseClick.x = LOWORD(lParam);
			lastMousePos.y = lastMouseClick.y = HIWORD(lParam);
			bool clickedOnButton = false;
			for (size_t i = 0; i < buttons.size(); ++i)
				if (buttons[i]->contains(lastMouseClick))
				{
					buttons[i]->onClick(lastMouseClick.x, lastMouseClick.y);
					clickedOnButton = true;
				}
			if (!clickedOnButton)
				track.leftPressed(lastMouseClick);
			if(objectToLabel)
			{
				objectToLabel->setBeingRelabeled(false);
				objectToLabel = NULL;
			}
			InvalidateRect(NULL, true);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (wParam == MK_LBUTTON && track.getMode())
			{
				long oldMouseX = lastMousePos.x;
				long oldMouseY = lastMousePos.y;
				lastMousePos.x = LOWORD(lParam);
				lastMousePos.y = HIWORD(lParam);
				RECT invalidateBounds;
				invalidateBounds.top = min(lastMousePos.y, lastMouseClick.y) - abs(oldMouseY - lastMousePos.y) - 8;
				invalidateBounds.bottom = max(lastMousePos.y, lastMouseClick.y) + abs(oldMouseY - lastMousePos.y) + 8;
				invalidateBounds.left = min(lastMousePos.x, lastMouseClick.x) - abs(oldMouseX - lastMousePos.x) - 8;
				invalidateBounds.right = max(lastMousePos.x, lastMouseClick.x) + abs(oldMouseX - lastMousePos.x) + 8;
				InvalidateRect(&invalidateBounds, true);
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			lastMousePos.x = LOWORD(lParam);
			lastMousePos.y = HIWORD(lParam);
			objectToLabel = track.leftReleased(lastMousePos);
			InvalidateRect(NULL, true);
		}
		break;
	case WM_RBUTTONUP:
		{
			POINT mouseClick;
			ReleaseCapture();
			mouseClick.x = LOWORD(lParam);
			mouseClick.y = HIWORD(lParam);
			track.rightReleased(mouseClick);
			track.removeDeletedElements();
			if(objectToLabel)
			{
				objectToLabel->setBeingRelabeled(false);
				objectToLabel = NULL;
			}
			InvalidateRect(NULL, true);
		}
		break;
	case WM_DESTROY: //When the window is destroyed
		PostQuitMessage(0);
		break;
	}
	return WndProcDefault(message, wParam, lParam);
}
