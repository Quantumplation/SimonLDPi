#include <vector>
#include "Redirect.h"
#include "NIDAQWrapper.h"
#include "Win32++\wincore.h"
#include "GUIBase.h"
#include "GUITrack.h"
#include "GUINode.h"
using namespace std;
using namespace SLDP;

#define TRANS(x, y) (x*xScale)+xShift, (y*yScale)+yShift
#define EDGE(x, y) track.addEdge(new GUIEdge(x, y))

class CView : public CWnd
{
public:
	CView() : nodeToLabel(NULL) {

		GUINode *L1, *L2, *L3;
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
		Empty1 = track.addNode(new GUINode(TRANS(10, 35)));
		Empty2 = track.addNode(new GUINode(TRANS(20, 30)));
		One1 = track.addNode(new GUINode(TRANS(10, 10), "ONE1"));
		One2 = track.addNode(new GUINode(TRANS(20, 10), "ONE2"));
		Empty3 = track.addNode(new GUINode(TRANS(30, 10)));
		Empty4 = track.addNode(new GUINode(TRANS(35, 0)));
		One3 = track.addNode(new GUINode(TRANS(10, 20), "ONE3"));
		One4 = track.addNode(new GUINode(TRANS(20, 20), "ONE4"));
		Two1 = track.addNode(new GUINode(TRANS(30, 20), "TWO1"));
		Two2 = track.addNode(new GUINode(TRANS(40, 20), "TWO2"));
		Two3 = track.addNode(new GUINode(TRANS(30, 30), "TWO3"));
		Two4 = track.addNode(new GUINode(TRANS(40, 30), "TWO4"));
		Three = track.addNode(new GUINode(TRANS(35, 30), "THREE"));
		Empty5 = track.addNode(new GUINode(TRANS(40, 35)));
		Empty6 = track.addNode(new GUINode(TRANS(60, 35)));
		Four = track.addNode(new GUINode(TRANS(45, 0), "FOUR"));
		Five1 = track.addNode(new GUINode(TRANS(50, 10), "FIVE1"));
		Five2 = track.addNode(new GUINode(TRANS(60, 10), "FIVE2"));
		Five3 = track.addNode(new GUINode(TRANS(50, 20), "FIVE3"));
		Five4 = track.addNode(new GUINode(TRANS(60, 20), "FIVE4"));
		Six = track.addNode(new GUINode(TRANS(65, 30), "SIX"));
		Seven1 = track.addNode(new GUINode(TRANS(70, 20), "SEVEN1"));
		Seven2 = track.addNode(new GUINode(TRANS(80, 20), "SEVEN2"));
		Seven3 = track.addNode(new GUINode(TRANS(70, 30), "SEVEN3"));
		Seven4 = track.addNode(new GUINode(TRANS(80, 30), "SEVEN4"));
		Empty7 = track.addNode(new GUINode(TRANS(80, 10)));
		Eight = track.addNode(new GUINode(TRANS(85, 0), "EIGHT"));
		R1 = track.addNode(new GUINode(TRANS(100, 0), "R1"));
		R2 = track.addNode(new GUINode(TRANS(100, 20), "R2"));
		Empty8 = track.addNode(new GUINode(TRANS(85, 30)));
		Empty9 = track.addNode(new GUINode(TRANS(95, 35)));
		R3 = track.addNode(new GUINode(TRANS(100, 35), "R3"));
		EDGE(L1, One1);
		EDGE(One1, One2);
		EDGE(One1, One4);
		EDGE(One3, One4);
		EDGE(One3, One2);
		EDGE(One2, Empty3);
		EDGE(Empty3, Empty4);
		EDGE(Empty4, Four);
		EDGE(Four, Eight);
		EDGE(Eight, R1);
		EDGE(Four, Five1);
		EDGE(Five1, Five2);
		EDGE(Five1, Five4);
		EDGE(Five3, Five4);
		EDGE(Five3, Five2);
		EDGE(Five2, Empty7);
		EDGE(Empty7, Eight);
		EDGE(L2, One3);
		EDGE(One4, Two1);
		EDGE(Two1, Two2);
		EDGE(Two1, Two4);
		EDGE(Two3, Two4);
		EDGE(Two3, Two2);
		EDGE(Two2, Five3);
		EDGE(Five4, Seven1);
		EDGE(Seven1, Seven2);
		EDGE(Seven1, Seven4);
		EDGE(Seven3, Seven4);
		EDGE(Seven3, Seven2);
		EDGE(Seven2, R2);
		EDGE(L3, Empty1);
		EDGE(Empty1, Empty2);
		EDGE(Empty2, Two3);
		EDGE(Two1, Two2);
		EDGE(Two1, Two4);
		EDGE(Two3, Three);
		EDGE(Three, Two4);
		EDGE(Two4, Six);
		EDGE(Three, Empty5);
		EDGE(Empty5, Empty6);
		EDGE(Empty6, Six);
		EDGE(Six, Seven3);
		EDGE(Seven4, Empty8);
		EDGE(Empty8, Empty9);
		EDGE(Empty9, R3); // Dear god this is so ugly.
	}
	virtual ~CView() {}
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC* dc)
	{
		track.draw(*dc);
		if (GetKeyState(VK_LBUTTON) & 0x8000)
		{
			dc->MoveTo(lastMouseClick);
			dc->LineTo(lastMousePos);
		}
	}


private:
	GUITrack track;
	POINT lastMouseClick;
	POINT lastMousePos;
	GUINode* nodeToLabel;
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
	NIDAQWrapper wrapper;
	wrapper.Initialize();
	MyApp myApp;
	return myApp.Run();
}

LRESULT CView::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_KEYDOWN:
		{
			if (nodeToLabel)
			{
				if (wParam >= '0' && wParam <= 'Z' && wParam != 0x40 || wParam == VK_SPACE)
					nodeToLabel->addCharToLabel((wchar_t)wParam);
				else if (wParam == VK_BACK)
					nodeToLabel->removeCharFromLabel();
				else if (wParam != VK_SHIFT)
				{
					nodeToLabel->setBeingRelabeled(false);
					nodeToLabel = NULL;
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
			track.leftPressed(lastMouseClick);
			if(nodeToLabel)
			{
				nodeToLabel->setBeingRelabeled(false);
				nodeToLabel = NULL;
			}
			InvalidateRect(NULL, true);
		}
		break;
	case WM_MOUSEMOVE:
		{
			if (wParam == MK_LBUTTON)
			{
				lastMousePos.x = LOWORD(lParam);
				lastMousePos.y = HIWORD(lParam);
				InvalidateRect(NULL, true);
			}
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			lastMousePos.x = LOWORD(lParam);
			lastMousePos.y = HIWORD(lParam);
			nodeToLabel = track.leftReleased(lastMousePos);
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
			if(nodeToLabel)
			{
				nodeToLabel->setBeingRelabeled(false);
				nodeToLabel = NULL;
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
