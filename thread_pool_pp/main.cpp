#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <algorithm>

enum class MessageTag
{
	Undefined = 0,

	Login = 1,
	ChatFromTo = 2,

	Count
};

struct ChatFromToMsg
{
	const MessageTag tag = MessageTag::ChatFromTo;

	std::string from;
	std::string to;
	std::string msg;

	std::string serialize()
	{
		return std::string(from + "::" + to + "::" + msg);
	}

	ChatFromToMsg deserialize(std::string buff)
	{
	}
};

void parse(std::string buff)
{
	// find first of ::
	int msgKind = 2;

	return ChatFromToMsg{"", "", ""};
}

void ChatFromToMsgHandler(ChatFromToMsg msg)
{
	User* u = map.lookup(msg.to);
	const auto serialized = msg.serialize();
	write(u.sockfd, serialized.data(), serialized.length()); 
}

void server()
{
	// listen();
	// accept();
	
	std::string buff;
	read(buf);

	auto msg = parse(buff);
	threadPool.submit(ChatFromToMsgHandler, msg);
}

class ThreadPool
{
	public:
		ThreadPool(const std::size_t count)
			: m_threadCount(std::max(2, count))
		{
			for (size_t idx = 0; idx < m_threadCount; idx++)
			{
				m_threads.push_back(std::thread([this] () {
					std::unique_lock<std::mutex> ul(m_mutex);
					m_cv.wait(ul, [this]() { return !m_queue.empty(); });

					auto fn = m_queue.front();
					m_queue.pop();
					ul.unlock();
					fn();
				}));
			}
		}

		template <typename Fn, typename... Args>
		void submit(Fn fn, Args... args)
		{
			auto lm = [fn, args...] () {
				fn(args...);
			};

			std::lock_guard<std::mutex> lg(m_mutex);
			m_queue.push(lm);
			m_cv.notify_one();
		}			

	private:
		const std::size_t m_threadCount;
		std::vector<std::thread> m_threads;
		std::mutex m_mutex;
		std::condition_variable m_cv;
		std::queue<std::function<void(void)>> m_queue;
};

int main()
{
}
