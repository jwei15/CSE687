/////////////////////////////////////////////////////////////////////
// FormWithCppThread.cpp - Demonstrate C++/CLI Child Thread        //
//                         and Form::Invoke                        //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2007       //
/////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Form1.h"

using namespace FormWithCppThread;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{

  // Enabling Windows XP visual effects before any controls are created
  Application::EnableVisualStyles();
  Application::SetCompatibleTextRenderingDefault(false); 
  // Create the main window and run it
  Application::Run(gcnew Form1());
  //Application::Run(gcnew Form1());
  return 0;
}

/*
public ref class MyapplicationContext : public System::Windows::Forms::ApplicationContext {
private: void onFormClosed(System::Object sender, EventArgs e) {
		if (Application::OpenForms.Count == 0) {
			ExitThread();
		}
	};
public: MyapplicationContext(void) {
		array<Form1^>^ forms = gcnew array<Form1^>() {
			gcnew Form1(), gcnew Form1()
		};
		for each(Form1^ form in forms) {
			form.FormClosed += onFormClosed;
		}
		for each(Form1^ form in forms) {
			form.Show();
		}
	}
};
*/
