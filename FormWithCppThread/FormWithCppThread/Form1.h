#pragma once
/////////////////////////////////////////////////////////////////////
// Form1.h - Demonstrate C++/CLI Child Thread and Form::Invoke     //
//                                                                 //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2007       //
/////////////////////////////////////////////////////////////////////

//#include "../../CodePublisher/CodePublisher.h"

namespace FormWithCppThread {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Threading;
	using namespace System::IO;

	using namespace System::Drawing;
	using namespace Microsoft::VisualBasic;


	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:

		Form1(void)
		{
			InitializeComponent();
		}

	protected:
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}
	private:
		System::Windows::Forms::Button^ convertButton;
		System::Windows::Forms::Button^  button1;
		System::Windows::Forms::ListBox^  listBox1;
		System::ComponentModel::Container ^components;
	private: System::Windows::Forms::Button^  button2;

			 bool dirty;

#pragma region Windows Form Designer generated code
			 /// <summary>
			 /// Required method for Designer support - do not modify
			 /// the contents of this method with the code editor.
			 /// </summary>
			 void InitializeComponent(void)
			 {
				 this->button1 = (gcnew System::Windows::Forms::Button());
				 this->listBox1 = (gcnew System::Windows::Forms::ListBox());
				 this->button2 = (gcnew System::Windows::Forms::Button());
				 this->SuspendLayout();
				 // 
				 // button1
				 // 
				 this->button1->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
				 this->button1->Location = System::Drawing::Point(263, 239);
				 this->button1->Name = L"button1";
				 this->button1->Size = System::Drawing::Size(190, 21);
				 this->button1->TabIndex = 0;
				 this->button1->Text = L"See Files in Current Directory";
				 this->button1->UseVisualStyleBackColor = true;
				 this->button1->Click += gcnew System::EventHandler(this, &Form1::button1_Click);
				 // 
				 // listBox1
				 // 
				 this->listBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
					 | System::Windows::Forms::AnchorStyles::Left)
					 | System::Windows::Forms::AnchorStyles::Right));
				 this->listBox1->FormattingEnabled = true;
				 this->listBox1->ItemHeight = 12;
				 this->listBox1->Location = System::Drawing::Point(12, 11);
				 this->listBox1->Name = L"listBox1";
				 this->listBox1->Size = System::Drawing::Size(460, 208);
				 this->listBox1->TabIndex = 1;
				 this->listBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::listBox1_SelectedIndexChanged);
				 this->listBox1->DoubleClick += gcnew System::EventHandler(this, &Form1::listBox1_DoubleClick);
				 // 
				 // button2
				 // 
				 this->button2->Anchor = System::Windows::Forms::AnchorStyles::Bottom;
				 this->button2->Location = System::Drawing::Point(33, 239);
				 this->button2->Name = L"button2";
				 this->button2->Size = System::Drawing::Size(190, 21);
				 this->button2->TabIndex = 2;
				 this->button2->Text = L"Give RE for file conversion";
				 this->button2->UseVisualStyleBackColor = true;
				 this->button2->Click += gcnew System::EventHandler(this, &Form1::button2_Click);
				 // 
				 // Form1
				 // 
				 this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
				 this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				 this->ClientSize = System::Drawing::Size(484, 299);
				 this->Controls->Add(this->button2);
				 this->Controls->Add(this->listBox1);
				 this->Controls->Add(this->button1);
				 this->Name = L"Form1";
				 this->Text = L"Form1";
				 this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
				 this->ResumeLayout(false);

			 }
#pragma endregion
			 //
	private:
		// delegate invokes functions that are registered with it when some
		// designated event occurs.  We do that here in response to a button
		// click event

		delegate Void itemDelegate(String^ str);  // defines type, itemDelegate
		itemDelegate^ myItemDelegate;             // instance, myItemDelegate

		//----< insert file names in listBox >---------------------------
		//
		//    must be called on Form's thread
		//
		Void addItem(String^ item)
		{
			this->listBox1->Items->Add(item);
		}
		//----< function processed by thread >---------------------------

		Void run()
		{
			if (dirty)
			{
				dirty = false;
				listBox1->Items->Clear();
			}
			else
			{
				dirty = true;
				array<String^>^ files = Directory::GetFiles(".");
				for each(String^ file in files)
				{
					String^ name = Path::GetFileName(file);

					// Form Invoke switches to UI thread
					if (this->InvokeRequired)
						this->Invoke(myItemDelegate, gcnew array<System::Object^, 1> { name });
					else
						this->listBox1->Items->Add(name);
				}
			}
		}
		//----< handle button click events >-----------------------------

		System::Void button1_Click(System::Object^  sender, System::EventArgs^  e)
		{
			Thread^ t = gcnew Thread(gcnew ThreadStart(this, &Form1::run));
			t->Start();
		}
		//----< register addItem function with delegate >----------------

		System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e)
		{
			myItemDelegate += gcnew itemDelegate(this, &Form1::addItem);
			this->Text = "Child Thread Demo";
			dirty = false;
		}
		//----< respond to double click in ListBox >---------------------

		System::Void listBox1_DoubleClick(System::Object^ sender, System::EventArgs^ e)
		{
			for (int i = 0; i < listBox1->Items->Count; ++i)
			{
				if (listBox1->GetSelected(i))
				{
					String^ msg = "selected ";
					msg += listBox1->Items[i];
					MessageBox::Show(msg, "dbl-click event");
					break;
				}
			}
		}
	private: System::Void listBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e) {
		String^ str = Microsoft::VisualBasic::Interaction::InputBox("Regular Expression for file selection", "Re InputBox", "Please input regular expression here", -1, -1);

		//Now str should hold a null string or the input regular expression

	}
	};
}

