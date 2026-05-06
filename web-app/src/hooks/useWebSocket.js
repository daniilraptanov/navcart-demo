import { useMemo } from 'react';
import useWebSocket, { ReadyState } from 'react-use-websocket';

export const useAppWebSocket = () => {
	const WS_URL = `ws://${window.location.hostname}:81`;

	const { sendJsonMessage, lastJsonMessage, readyState } = useWebSocket(
		WS_URL,
		{
			shouldReconnect: () => true,
			reconnectAttempts: 10,
			reconnectInterval: 3000,
			// onOpen: () => console.log('web socket opened'),
			// onClose: () => console.log('web socket closed'),
		}
	);

	const connectionStatus = useMemo(() => {
		return {
			[ReadyState.CONNECTING]: 'Connecting... ⏳',
			[ReadyState.OPEN]: 'Connected 🟢',
			[ReadyState.CLOSING]: 'Disconnecting... 🟡',
			[ReadyState.CLOSED]: 'Disconnected 🔴',
			[ReadyState.UNINSTANTIATED]: 'Waiting...',
		}[readyState];
	}, [readyState]);

	return {
		send: sendJsonMessage,
		lastMessage: lastJsonMessage,
		statusText: connectionStatus,
		isConnected: readyState === ReadyState.OPEN,
	};
};
