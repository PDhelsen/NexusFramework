#include "NexusFramework/Core/NexusFrameworkCore.h"
#include "NexusFramework/IO/File.h"

namespace NxFr
{
	namespace Streaming
	{
		using Default = class Line;

		template<typename T>
		class StreamingAlgorithm
		{
		public:
			using Block = T;

			StreamingAlgorithm(StringView Path)
				: Handle(Path), FileMode(File::Mode::Read)
			{
			}

			~StreamingAlgorithm()
			{
			}

			void Open(File::Mode Mode)
			{
				FileMode = Mode;
				Handle.Open(Mode, Mode == File::Mode::Write || Mode == File::Mode::Append);

				if (FileMode == File::Mode::Read)
				{
					Cache();
				}
			}

			void Close()
			{
				if (FileMode == File::Mode::Write || FileMode == File::Mode::Append)
				{
					Flush();
				}

				Handle.Close();
			}

			virtual void Cache() = 0;
			virtual void Flush() = 0;

			virtual Block Read() = 0;
			virtual void Write(Block Data) = 0;

			StringView GetPath() const { return Handle.GetPath(); }
			File::Mode GetMode() const { return FileMode; }
			bool IsOpened() const { return Handle.IsOpened(); }

		protected:
			File Handle;
			File::Mode FileMode;
		};

		class Line : public StreamingAlgorithm<StringView>
		{
		public:
			NEXUS_FRAMEWORK_API Line(StringView Path);
			NEXUS_FRAMEWORK_API ~Line();

			NEXUS_FRAMEWORK_API void Cache() override;
			NEXUS_FRAMEWORK_API void Flush() override;

			NEXUS_FRAMEWORK_API Block Read() override;
			NEXUS_FRAMEWORK_API void Write(Block Data) override;

		private:
			String Buffer;
			uint64 Cursor;
		};
	}

	template<typename S = Streaming::Default>
	class Stream
	{
	public:
		Stream(StringView Path) : State(Path) { }
		~Stream() { }

		void Open(File::Mode Mode) { State.Open(Mode); }
		void Close() { State.Close(); }

		void Cache() { State.Cache(); }
		void Flush() { State.Flush(); }

		typename S::Block Read() { return State.Read(); }
		void Write(typename S::Block Data) { State.Write(Data); }

		StringView GetPath() const { return State.GetPath(); }
		File::Mode GetMode() const { return State.GetMode(); }
		bool IsOpened() const { return State.IsOpened(); }

	private:
		S State;
	};
}
